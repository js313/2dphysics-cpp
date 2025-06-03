#include "./Force.h"
#include "./Constants.h"
#include <algorithm>

Vec2 Force::GenerateDragForce(const Body &body, float k)
{
    if (body.velocity.MagnitudeSquared() == 0.0)
        return Vec2(0, 0);

    Vec2 dragDirection = body.velocity.UnitVector() * -1;
    float dragMagnitude = k * body.velocity.MagnitudeSquared();

    return dragDirection * dragMagnitude;
}

Vec2 Force::GenerateFrictionForce(const Body &body, float k)
{
    Vec2 frictionDirection = body.velocity.UnitVector() * -1;
    float frictionMagnitude = k;

    return frictionDirection * frictionMagnitude;
}

Vec2 Force::GenerateGravitationForce(const Body &a, const Body &b, float G, float minDistance, float maxDistance)
{
    Vec2 d = b.position - a.position; // from a->b

    Vec2 attractionDirection = d.UnitVector();
    float distanceSquared = d.MagnitudeSquared();
    distanceSquared = std::clamp(distanceSquared, minDistance, maxDistance);
    float attractionMagnitude = G * a.mass * b.mass / distanceSquared;
    return attractionDirection * attractionMagnitude;
}

Vec2 Force::GenerateSpringForce(const Body &body, const Vec2 &anchor, float restLength, float k)
{
    Vec2 d = body.position - anchor;
    float displacement = d.Magnitude() - restLength;

    Vec2 springDirection = d.UnitVector();
    float springMagnitude = -k * displacement;
    return springDirection * springMagnitude;
}

Vec2 Force::GenerateSpringForce(const Body &a, const Body &b, float restLength, float k)
{
    Vec2 d = a.position - b.position;
    float displacement = d.Magnitude() - restLength;

    Vec2 springDirection = d.UnitVector();
    float springMagnitude = -k * displacement;
    return springDirection * springMagnitude;
}