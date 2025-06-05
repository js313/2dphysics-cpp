#include <iostream>
#include "Body.h"

Body::Body(Shape *shape, float x, float y, float mass)
{
    this->shape = shape;
    this->position = Vec2(x, y);
    this->velocity = Vec2(0, 0);
    this->acceleration = Vec2(0, 0);
    this->rotation = 0.0;
    this->angularVelocity = 0.0;
    this->angularAcceleration = 0.0;
    this->sumForces = Vec2(0, 0);
    this->sumTorques = 0.0;
    this->mass = mass;
    if (mass == 0.0)
        this->invMass = 0.0;
    else
        this->invMass = 1.0 / mass;

    this->I = this->shape->GetMoIPerUnitMass() * this->mass;
    if (I == 0.0)
        this->invI = 0.0;
    else
        this->invI = 1.0 / I;
    std::cout << "Body constructor called!\n";
}

Body::~Body()
{
    delete shape;
    std::cout << "Body destructor called!\n";
}

void Body::IntegrateLinear(float dt)
{
    // Find acceleration based on net of all forces applied
    acceleration = sumForces * invMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;
    // Integrate the velocity to find the new position
    position += velocity * dt;

    ClearForces();
}

void Body::IntegrateAngular(float dt)
{
    // Find angular acceleration based on net of all forces applied
    angularAcceleration = sumTorques * invI;

    // Integrate the angular acceleration to find the new angular velocity
    angularVelocity += angularAcceleration * dt;
    // Integrate the angular velocity to find the new rotation angle(theta)
    rotation += angularVelocity * dt;

    ClearTorques();
}

void Body::AddForce(const Vec2 &force)
{
    sumForces += force;
}

void Body::AddTorque(float torque)
{
    sumTorques += torque;
}

void Body::ClearForces()
{
    sumForces *= 0;
}

void Body::ClearTorques()
{
    sumTorques *= 0;
}