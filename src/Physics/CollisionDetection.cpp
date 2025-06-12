#include "./CollisionDetection.h"

bool CollisionDetection::IsColliding(const Body *const a, const Body *const b, Contact &contact)
{
    Shape *shapeA = a->shape;
    Shape *shapeB = b->shape;

    if (shapeA->GetType() == CIRCLE && shapeB->GetType() == CIRCLE)
    {
        return IsCollidingCircleCircle(a, b, contact);
    }
    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(const Body *const a, const Body *const b, Contact &contact)
{
    Shape *shapeA = a->shape;
    Shape *shapeB = b->shape;

    float circleARadius = ((CircleShape *)shapeA)->radius;
    float circleBRadius = ((CircleShape *)shapeB)->radius;

    Vec2 ab = b->position - a->position;

    float centreDistanceSquared = ab.MagnitudeSquared();

    contact.a = a;
    contact.b = b;

    contact.normal = ab.Normalize();
    // addition of a's or b's position gives the world space coordinates of the points
    contact.start = b->position - contact.normal * circleBRadius;
    contact.end = a->position + contact.normal * circleARadius;

    contact.depth = (contact.end - contact.start).Magnitude();

    return centreDistanceSquared <= (circleARadius + circleBRadius) * (circleARadius + circleBRadius);
}