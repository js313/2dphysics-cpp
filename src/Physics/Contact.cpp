#include "./Contact.h"

void Contact::ResolvePenetration()
{
    if (a->IsStatic() && b->IsStatic())
        return;
    float da = (depth * (a->invMass)) / (a->invMass + b->invMass);
    float db = (depth * (b->invMass)) / (a->invMass + b->invMass);

    a->position -= normal * da;
    b->position += normal * db;
}

void Contact::ResolveCollision()
{
    ResolvePenetration();

    float e = std::min(a->restitution, b->restitution);

    const Vec2 vrel = a->velocity - b->velocity;

    float vrelDotNormal = vrel.Dot(normal);

    const Vec2 impulseDirection = normal;
    const float impulseMagnitude = -(1 + e) * vrelDotNormal / (a->invMass + b->invMass);

    Vec2 j = impulseDirection * impulseMagnitude;

    a->ApplyImpulse(j);
    b->ApplyImpulse(-j);
}