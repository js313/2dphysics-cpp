#include "./CollisionDetection.h"

bool CollisionDetection::IsColliding(const Body *const a, const Body *const b)
{
    Shape *shapeA = a->shape;
    Shape *shapeB = b->shape;

    if (shapeA->GetType() == CIRCLE && shapeB->GetType() == CIRCLE)
    {
        return IsCollidingCircleCircle(a, b);
    }
    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(const Body *const a, const Body *const b)
{
    Shape *shapeA = a->shape;
    Shape *shapeB = b->shape;

    float circleARadius = ((CircleShape *)shapeA)->radius;
    float circleBRadius = ((CircleShape *)shapeB)->radius;

    float centreDistanceSquared = (b->position - a->position).MagnitudeSquared();

    return centreDistanceSquared <= (circleARadius + circleBRadius) * (circleARadius + circleBRadius);
}