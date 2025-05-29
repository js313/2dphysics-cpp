#include "./Force.h"

Vec2 Force::GenerateDragForce(const Particle &particle, float k)
{
    if (particle.velocity.MagnitudeSquared() == 0.0)
        return Vec2(0, 0);

    Vec2 dragDirection = particle.velocity.UnitVector() * -1;
    float dragMagnitude = k * particle.velocity.MagnitudeSquared();

    return dragDirection * dragMagnitude;
}