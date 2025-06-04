#include <iostream>
#include "Body.h"

Body::Body(Shape *shape, float x, float y, float mass)
{
    this->shape = shape;
    this->position = Vec2(x, y);
    this->mass = mass;
    if (mass == 0.0)
        this->invMass = 0.0;
    else
        this->invMass = 1.0 / mass;
    std::cout << "Body constructor called!\n";
}

Body::~Body()
{
    delete shape;
    std::cout << "Body destructor called!\n";
}

void Body::Integrate(float dt)
{
    // Find acceleration based on net of all forces applied
    acceleration = sumForces * invMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;
    // Integrate the velocity to find the new position
    position += velocity * dt;

    ClearForces();
}

void Body::AddForce(const Vec2 &force)
{
    sumForces += force;
}

void Body::ClearForces()
{
    sumForces *= 0;
}