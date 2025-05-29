#include <iostream>
#include "Particle.h"

Particle::Particle(float x, float y, float mass, float radius)
{
    this->position = Vec2(x, y);
    this->mass = mass;
    if (mass == 0.0)
        this->invMass = 0.0;
    else
        this->invMass = 1.0 / mass;
    this->radius = radius;
    std::cout << "Particle constructor called!\n";
}

Particle::~Particle()
{
    std::cout << "Particle destructor called!\n";
}

void Particle::Integrate(float dt)
{
    // Find acceleration based on net of all forces applied
    acceleration = sumForces * invMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;
    // Integrate the velocity to find the new position
    position += velocity * dt;

    ClearForces();
}

void Particle::AddForce(const Vec2 &force)
{
    sumForces += force;
}

void Particle::ClearForces()
{
    sumForces *= 0;
}