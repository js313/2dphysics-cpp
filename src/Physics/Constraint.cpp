#include "Constraint.h"

MatMN Constraint::GetInvM()
{
    MatMN invM = MatMN(6, 6);

    invM.Zero();
    invM.rows[0][0] = a->invMass;
    invM.rows[1][1] = a->invMass;
    invM.rows[2][2] = a->invI;
    invM.rows[3][3] = b->invMass;
    invM.rows[4][4] = b->invMass;
    invM.rows[5][5] = b->invI;

    return invM;
}

VecN Constraint::GetVelocities()
{
    VecN V(6);
    V.Zero();
    V[0] = a->velocity.x;
    V[1] = a->velocity.y;
    V[2] = a->angularVelocity;
    V[3] = b->velocity.x;
    V[4] = b->velocity.y;
    V[5] = b->angularVelocity;
    return V;
}

JointConstraint::JointConstraint() : Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.0f)
{
    cachedLambda.Zero();
};
JointConstraint::JointConstraint(Body *a, Body *b, const Vec2 &anchorPoint) : Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.0f)
{
    this->a = a;
    this->b = b;
    aPoint = this->a->WorldSpaceToLocalSpace(anchorPoint);
    bPoint = this->b->WorldSpaceToLocalSpace(anchorPoint);
    cachedLambda.Zero();
};

void JointConstraint::PreSolve(const float dt)
{
    const Vec2 pa = a->LocalSpaceToWorldSpace(aPoint);
    const Vec2 pb = b->LocalSpaceToWorldSpace(bPoint);

    const Vec2 ra = pa - a->position;
    const Vec2 rb = pb - b->position;

    jacobian.Zero();

    Vec2 J1 = (pa - pb) * 2.0;
    jacobian.rows[0][0] = J1.x; // A linear velocity.x
    jacobian.rows[0][1] = J1.y; // A linear velocity.y

    float J2 = ra.Cross(pa - pb) * 2.0;
    jacobian.rows[0][2] = J2; // A angular velocity

    Vec2 J3 = (pb - pa) * 2.0;
    jacobian.rows[0][3] = J3.x; // B linear velocity.x
    jacobian.rows[0][4] = J3.y; // B linear velocity.y

    float J4 = rb.Cross(pb - pa) * 2.0;
    jacobian.rows[0][5] = J4; // B angular velocity

    // Warm starting (apply cached lambda)
    const MatMN Jt = jacobian.Transpose();
    VecN impulses = Jt * cachedLambda;

    // Apply the impulses to both bodies
    a->ApplyImpulseLinear(Vec2(impulses[0], impulses[1])); // A linear impulse
    a->ApplyImpulseAngular(impulses[2]);                   // A angular impulse
    b->ApplyImpulseLinear(Vec2(impulses[3], impulses[4])); // B linear impulse
    b->ApplyImpulseAngular(impulses[5]);                   // B angular impulse

    // Compute the bias term (baumgarte stabilization)
    const float beta = 0.02f;
    float C = (pb - pa).Dot(pb - pa);
    C = std::max(0.0f, C - 0.01f);
    bias = (beta / dt) * C;
}

void JointConstraint::Solve()
{
    const VecN V = GetVelocities();
    const MatMN invM = GetInvM();

    const MatMN J = jacobian;
    const MatMN Jt = jacobian.Transpose();

    // Compute lambda using Ax=b (Gauss-Seidel method)
    MatMN lhs = J * invM * Jt; // A
    VecN rhs = J * V * -1.0f;  // b
    rhs[0] -= bias;
    VecN lambda = MatMN::SolveGaussSeidel(lhs, rhs);
    cachedLambda += lambda;

    // Compute the impulses with both direction and magnitude
    VecN impulses = Jt * lambda;

    // Apply the impulses to both bodies
    a->ApplyImpulseLinear(Vec2(impulses[0], impulses[1])); // A linear impulse
    a->ApplyImpulseAngular(impulses[2]);                   // A angular impulse
    b->ApplyImpulseLinear(Vec2(impulses[3], impulses[4])); // B linear impulse
    b->ApplyImpulseAngular(impulses[5]);                   // B angular impulse
}