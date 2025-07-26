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

JointConstraint::JointConstraint() : Constraint(), jacobian(1, 6) {};
JointConstraint::JointConstraint(Body *a, Body *b, const Vec2 &anchorPoint) : Constraint(), jacobian(1, 6)
{
    this->a = a;
    this->b = b;
    aPoint = this->a->WorldSpaceToLocalSpace(anchorPoint);
    bPoint = this->b->WorldSpaceToLocalSpace(anchorPoint);
};