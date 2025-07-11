#include "./Contact.h"

void Contact::ResolvePenetration()
{
    if (a->IsStatic() && b->IsStatic())
        return;
    float da = (depth * (a->invMass)) / (a->invMass + b->invMass);
    float db = (depth * (b->invMass)) / (a->invMass + b->invMass);

    a->position -= normal * da;
    b->position += normal * db;

    a->shape->UpdateVertices(a->rotation, a->position);
    b->shape->UpdateVertices(b->rotation, b->position);
}

void Contact::ResolveCollision()
{
    ResolvePenetration();

    float e = std::min(a->restitution, b->restitution);
    float f = std::min(a->friction, b->friction);

    Vec2 ra = end - a->position;
    Vec2 rb = start - b->position;
    Vec2 va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
    Vec2 vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);
    const Vec2 vrel = va - vb;

    float vrelDotNormal = vrel.Dot(normal);

    const Vec2 impulseDirectionN = normal;
    // Simplified version for 2D physics, as Cross products don't make complete sense in only 2 dimesnsions
    const float impulseMagnitudeN = -(1 + e) * vrelDotNormal / ((a->invMass + b->invMass) + ra.Cross(normal) * ra.Cross(normal) * a->invI + rb.Cross(normal) * rb.Cross(normal) * b->invI);

    Vec2 jn = impulseDirectionN * impulseMagnitudeN;

    Vec2 tangent = normal.Normal();
    float vrelDotTangent = vrel.Dot(tangent);

    const Vec2 impulseDirectionT = tangent;
    const float impulseMagnitudeT = f * -(1 + e) * vrelDotTangent / ((a->invMass + b->invMass) + ra.Cross(tangent) * ra.Cross(tangent) * a->invI + rb.Cross(tangent) * rb.Cross(tangent) * b->invI);

    Vec2 jt = impulseDirectionT * impulseMagnitudeT;

    Vec2 j = jn + jt;

    a->ApplyImpulse(j, ra);
    b->ApplyImpulse(-j, rb);
}