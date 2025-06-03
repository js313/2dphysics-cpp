#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include "Vec2.h"

enum ShapeType
{
    CIRCLE,
    POLYGON,
    BOX
};

struct Shape // abstract struct, as there is one(or more) pure virtual function
{
    virtual ~Shape() = default;
    // this is a pure virtual function
    virtual ShapeType GetType() const = 0; // "const" means The object this is called on, it's properties won't be modified
};

struct CircleShape : public Shape
{
    float radius;

    CircleShape(float radius);
    virtual ~CircleShape();
    ShapeType GetType() const override;
};

struct PolygonShape : public Shape
{
    std::vector<Vec2> vertices;

    PolygonShape(const std::vector<Vec2> vertices);
    virtual ~PolygonShape();
    ShapeType GetType() const override;
};

struct BoxShape : public PolygonShape
{
    float width;
    float height;

    BoxShape(float width, float height);
    virtual ~BoxShape();
    ShapeType GetType() const override; // Won't work if the parent class uses the final keyword while overriding
};

#endif