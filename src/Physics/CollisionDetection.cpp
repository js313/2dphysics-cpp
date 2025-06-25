#include "./CollisionDetection.h"

bool CollisionDetection::IsColliding(Body *const a, Body *const b, Contact &contact)
{
    Shape *shapeA = a->shape;
    Shape *shapeB = b->shape;

    if (shapeA->GetType() == CIRCLE && shapeB->GetType() == CIRCLE)
    {
        return IsCollidingCircleCircle(a, b, contact);
    }
    else if ((shapeA->GetType() == POLYGON || shapeA->GetType() == BOX) && (shapeB->GetType() == POLYGON || shapeB->GetType() == BOX))
    {
        return IsCollidingPolygonPolygon(a, b, contact);
    }
    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body *const a, Body *const b, Contact &contact)
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

bool CollisionDetection::IsCollidingPolygonPolygon(Body *const a, Body *const b, Contact &contact)
{
    const PolygonShape *aPolygonShape = (PolygonShape *)a->shape;
    const PolygonShape *bPolygonShape = (PolygonShape *)b->shape;

    Vec2 aAxis, aStartPoint;
    Vec2 bAxis, bStartPoint;

    float abSeperation = aPolygonShape->FindMinSeperation(*bPolygonShape, aAxis, aStartPoint);
    if (abSeperation >= 0)
        return false;

    float baSeperation = bPolygonShape->FindMinSeperation(*aPolygonShape, bAxis, bStartPoint);
    if (baSeperation >= 0)
        return false;

    contact.a = a;
    contact.b = b;
    // If body a's edge is involved in the collision then abSeperation will have higher value
    // else baSeperation will have a higher value, as in the FindMinSeperation function we are assuming that
    // body a's(the body passed as first argument) edge is the one with colliding edge and the other one
    // has a colliding vertex
    if (abSeperation >= baSeperation)
    {
        contact.depth = -abSeperation;
        contact.normal = aAxis.Normal();
        contact.start = aStartPoint;
        contact.end = contact.start + (contact.normal * contact.depth);
    }
    else if (abSeperation < baSeperation)
    {
        contact.depth = -baSeperation;
        contact.normal = -bAxis.Normal();
        contact.end = bStartPoint;
        contact.start = contact.end - (contact.normal * contact.depth);
    }

    return true;
}