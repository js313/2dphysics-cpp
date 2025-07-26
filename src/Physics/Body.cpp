#include <iostream>
#include "Body.h"
#include "../Graphics.h"

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
    this->restitution = 1.0;
    this->friction = 0.7;

    std::cout << "Body constructor called!\n";
}

Body::~Body()
{
    delete shape;
    std::cout << "Body destructor called!\n";
}

void Body::SetTexture(const char *textureFileName)
{
    SDL_Surface *surface = IMG_Load(textureFileName);
    if (surface)
    {
        texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void Body::IntegrateLinear(float dt)
{
    if (IsStatic())
        return;

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
    if (IsStatic())
        return;

    // Find angular acceleration based on net of all forces applied
    angularAcceleration = sumTorques * invI;

    // Integrate the angular acceleration to find the new angular velocity
    angularVelocity += angularAcceleration * dt;
    // Integrate the angular velocity to find the new rotation angle(theta)
    rotation += angularVelocity * dt;

    ClearTorques();
}

void Body::Update(float dt)
{
    IntegrateLinear(dt);
    IntegrateAngular(dt);
    shape->UpdateVertices(rotation, position);
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

bool Body::IsStatic() const
{
    float epsilon = 0.005f;
    return fabs(invMass - 0.0f) < epsilon;
}

void Body::ApplyImpulse(const Vec2 &j)
{
    if (IsStatic())
        return;

    velocity += j * invMass;
}

void Body::ApplyImpulse(const Vec2 &j, const Vec2 &r)
{
    if (IsStatic())
        return;

    velocity += j * invMass;
    angularVelocity += r.Cross(j) * invI;
}

Vec2 Body::LocalSpaceToWorldSpace(const Vec2 &point)
{
    Vec2 rotated = point.Rotate(this->rotation);
    return rotated + position;
}

Vec2 Body::WorldSpaceToLocalSpace(const Vec2 &point)
{
    float translatedX = point.x - position.x;
    float translatedY = point.y - position.y;
    float rotatedX = cos(-rotation) * translatedX - sin(-rotation) * translatedY;
    float rotatedY = cos(-rotation) * translatedY + sin(-rotation) * translatedX;

    return Vec2(rotatedX, rotatedY);
}