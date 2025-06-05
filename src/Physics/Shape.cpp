#include "Shape.h"
#include <iostream>

CircleShape::CircleShape(float radius)
{
    this->radius = radius;
    std::cout << "CircleShape constructor called!\n";
}

CircleShape::~CircleShape()
{
    std::cout << "CircleShape destructor called!\n";
}

ShapeType CircleShape::GetType() const
{
    return CIRCLE;
}

float CircleShape::GetMoIPerUnitMass() const
{
    return 0.5 * this->radius * this->radius;
}

PolygonShape::PolygonShape(const std::vector<Vec2> vertices)
{
    this->vertices = vertices;
    std::cout << "PolygonShape constructor called!\n";
}

PolygonShape::~PolygonShape()
{
    std::cout << "PolygonShape destructor called!\n";
}

ShapeType PolygonShape::GetType() const
{
    return POLYGON;
}

float PolygonShape::GetMoIPerUnitMass() const
{
    return 0.0;
}

BoxShape::BoxShape(float width, float height) : PolygonShape({})
{
    this->width = width;
    this->height = height;
    std::cout << "BoxShape constructor called!\n";
}

BoxShape::~BoxShape()
{
    std::cout << "BoxShape destructor called!\n";
}

ShapeType BoxShape::GetType() const
{
    return BOX;
}

float BoxShape::GetMoIPerUnitMass() const
{
    return (1 / 12.0) * this->width * this->height;
}