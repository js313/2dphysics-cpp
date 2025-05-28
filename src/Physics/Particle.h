#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

struct Particle
{
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    Vec2 sumForces;

    float radius;
    float mass;

    Particle(float x, float y, float mass, float radius);
    ~Particle();

    void Integrate(float dt);
    void AddForce(const Vec2 &force);
    void ClearForces();
};

#endif