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
    this->localVertices = vertices;
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
    // Load vertices based on local space(considering (0,0) as origin)
    this->localVertices.push_back(Vec2(-this->width / 2.0, -this->height / 2.0));
    this->localVertices.push_back(Vec2(+this->width / 2.0, -this->height / 2.0));
    this->localVertices.push_back(Vec2(+this->width / 2.0, +this->height / 2.0));
    this->localVertices.push_back(Vec2(-this->width / 2.0, +this->height / 2.0));

    this->globalVertices.push_back(Vec2(-this->width / 2.0, -this->height / 2.0));
    this->globalVertices.push_back(Vec2(+this->width / 2.0, -this->height / 2.0));
    this->globalVertices.push_back(Vec2(+this->width / 2.0, +this->height / 2.0));
    this->globalVertices.push_back(Vec2(-this->width / 2.0, +this->height / 2.0));
    std::cout << "BoxShape constructor called!\n";
}

void PolygonShape::UpdateVertices(float rotation, const Vec2 &position)
{
    for (int i = 0; i < localVertices.size(); i++)
    {
        // First rotation, then translation(as rotaion works only if about origin)
        globalVertices[i] = localVertices[i].Rotate(rotation);
        globalVertices[i] += position;
    }
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