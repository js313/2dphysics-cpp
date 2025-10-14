#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Body.h"
#include "MatMN.h"
#include "VecN.h"

class Constraint
{
public:
    Body *a;
    Body *b;

    // Anchor points in local space for respective bodies
    Vec2 aPoint;
    Vec2 bPoint;

    virtual ~Constraint() = default;

    MatMN GetInvM();
    VecN GetVelocities();

    virtual void Solve() {};
};

class JointConstraint : public Constraint
{
    MatMN jacobian;

public:
    JointConstraint();
    JointConstraint(Body *a, Body *b, const Vec2 &anchorPoint);
    void Solve() override;
};

class PenetrationConstraint : public Constraint
{
    MatMN jacobian;

public:
    // void Solve() override;
};

#endif