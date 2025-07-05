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
    else if ((shapeA->GetType() == POLYGON || shapeA->GetType() == BOX) && shapeB->GetType() == CIRCLE)
    {
        return IsCollidingCirclePolygon(a, b, contact);
    }
    else if ((shapeB->GetType() == POLYGON || shapeB->GetType() == BOX) && shapeA->GetType() == CIRCLE)
    {
        return IsCollidingCirclePolygon(b, a, contact);
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

bool CollisionDetection::IsCollidingCirclePolygon(Body *const polygon, Body *const circle, Contact &contact)
{
    PolygonShape *polygonShape = (PolygonShape *)polygon->shape;
    CircleShape *circleShape = (CircleShape *)circle->shape;

    bool isOutside = false;
    Vec2 minCurrVertex;
    Vec2 minNextVertex;
    float distanceCircleEdge = std::numeric_limits<float>::lowest();

    std::vector<Vec2> &polygonVertices = polygonShape->globalVertices;
    for (int i = 0; i < polygonVertices.size(); i++)
    {
        int currentVertex = i;
        int nextVertex = (i + 1) % polygonVertices.size();
        Vec2 edge = polygonShape->EdgeAt(currentVertex);
        Vec2 normal = edge.Normal();

        Vec2 vertexToCircleCenter = circle->position - polygonVertices[currentVertex];
        float projection = vertexToCircleCenter.Dot(normal);

        if (projection > 0)
        {
            distanceCircleEdge = projection;
            minCurrVertex = polygonVertices[currentVertex];
            minNextVertex = polygonVertices[nextVertex];
            isOutside = true;
            break;
        }
        else
        {
            if (projection > distanceCircleEdge)
            {
                distanceCircleEdge = projection;
                minCurrVertex = polygonVertices[currentVertex];
                minNextVertex = polygonVertices[nextVertex];
            }
        }
    }

    if (isOutside)
    {
        Vec2 v1 = circle->position - minCurrVertex;
        Vec2 v2 = minNextVertex - minCurrVertex;
        if (v1.Dot(v2) < 0)
        {
            if (v1.Magnitude() > circleShape->radius)
                return false;
            else
            {
                contact.a = polygon;
                contact.b = circle;
                contact.depth = circleShape->radius - v1.Magnitude();
                contact.normal = v1.Normalize();
                contact.start = circle->position + (contact.normal * -circleShape->radius);
                contact.end = contact.start + (contact.normal * contact.depth);
            }
        }
        else
        {
            Vec2 v1 = circle->position - minNextVertex;
            Vec2 v2 = minCurrVertex - minNextVertex;
            if (v1.Dot(v2) < 0)
            {
                if (v1.Magnitude() > circleShape->radius)
                    return false;
                else
                {
                    contact.a = polygon;
                    contact.b = circle;
                    contact.depth = circleShape->radius - v1.Magnitude();
                    contact.normal = v1.Normalize();
                    contact.start = circle->position + (contact.normal * -circleShape->radius);
                    contact.end = contact.start + (contact.normal * contact.depth);
                }
            }
            else
            {
                if (distanceCircleEdge > circleShape->radius)
                    return false;
                else
                {
                    contact.a = polygon;
                    contact.b = circle;
                    contact.depth = circleShape->radius - distanceCircleEdge;
                    contact.normal = (minNextVertex - minCurrVertex).Normal();
                    contact.start = circle->position + (contact.normal * -circleShape->radius);
                    contact.end = contact.start + (contact.normal * contact.depth);
                }
            }
        }
    }
    else // circle's centre is inside polygon
    {
        contact.a = polygon;
        contact.b = circle;
        contact.depth = circleShape->radius - distanceCircleEdge;
        contact.normal = (minNextVertex - minCurrVertex).Normal();
        contact.start = circle->position + (contact.normal * -circleShape->radius);
        contact.end = contact.start + (contact.normal * contact.depth);
    }
    return true;
}