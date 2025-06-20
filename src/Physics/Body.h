#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
#include "Shape.h"

struct Body
{
    bool isColliding;

    // Linear motion
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    // Angular motion
    float rotation;
    float angularVelocity;
    float angularAcceleration;

    Vec2 sumForces;
    float sumTorques;

    // mass
    float mass;
    float invMass;
    // moment of inertia
    float I;
    float invI;

    float restitution;

    Shape *shape;

    Body(Shape *shape, float x, float y, float mass);
    ~Body();

    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);

    void AddForce(const Vec2 &force);
    void AddTorque(float torque);

    void ClearForces();
    void ClearTorques();

    void Update(float dt);

    bool IsStatic() const;
    void MoveBy(const Vec2 &displacement) const;
    void ApplyImpulse(const Vec2 &j);
};

#endif