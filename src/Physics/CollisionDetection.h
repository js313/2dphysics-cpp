#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "./Body.h"
#include "./Shape.h"
#include "./Contact.h"

struct CollisionDetection
{
    static bool IsColliding(const Body *const a, const Body *const b, Contact &contact);

    static bool IsCollidingCircleCircle(const Body *const a, const Body *const b, Contact &contact);

    // TODO:
    // static void IsCollidingCirclePolygon(Body *a, Body *b);
    // static void IsCollidingPolygonPolygon(Body *a, Body *b);
};

#endif