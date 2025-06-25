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

    Vec2 ra = end - a->position;
    Vec2 rb = start - b->position;
    Vec2 va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
    Vec2 vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);
    const Vec2 vrel = va - vb;

    float vrelDotNormal = vrel.Dot(normal);

    const Vec2 impulseDirection = normal;
    // Simplified version for 2D physics, as Cross products don't make complete sense in only 2 dimesnsions
    const float impulseMagnitude = -(1 + e) * vrelDotNormal / ((a->invMass + b->invMass) + ra.Cross(normal) * ra.Cross(normal) * a->invI + rb.Cross(normal) * rb.Cross(normal) * b->invI);

    Vec2 j = impulseDirection * impulseMagnitude;

    a->ApplyImpulse(j, ra);
    b->ApplyImpulse(-j, rb);
}