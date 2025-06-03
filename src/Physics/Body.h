#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
#include "Shape.h"

struct Body
{
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    Vec2 sumForces;

    float radius;
    float mass;
    float invMass;

    Shape *shape;

    Body(Shape *shape, float x, float y, float mass, float radius);
    ~Body();

    void Integrate(float dt);
    void AddForce(const Vec2 &force);
    void ClearForces();
};

#endif