#include <iostream>
#include "World.h"
#include "Constants.h"
#include "Contact.h"
#include "CollisionDetection.h"

World::World(float gravity)
{
    G = -gravity; // in sdl2 y coordinate increases in the downward
    std::cout << "World constructor called!\n";
}

World::~World()
{
    for (auto &body : bodies)
        delete (body);
    std::cout << "World destructor called!\n";
}

void World::AddBody(Body *body)
{
    bodies.push_back(body);
}

std::vector<Body *> &World::GetBodies()
{
    return bodies;
}

void World::AddForce(const Vec2 &force)
{
    forces.push_back(force);
}

void World::AddTorque(float torque)
{
    torques.push_back(torque);
}

void World::Update(float dt)
{
    for (auto body : bodies)
    {
        // Weight
        body->AddForce(Vec2(0, body->mass * G * PIXELS_PER_METRE));
        // Push
        // body->AddForce(pushForce);
        // if (body->position.y > liquid.y)
        // Drag
        // body->AddForce(Force::GenerateDragForce(*body, 0.01));
        // else
        // Wind
        // body->AddForce(Vec2(10 * PIXELS_PER_METRE, 0));
        // Friction
        // body->AddForce(Force::GenerateFrictionForce(*body, 10.0));
        // Drag
        // body->AddForce(Force::GenerateDragForce(*body, 0.003));
        // Torque
        // body->AddTorque(200.0);
        // Spring
        // body->AddForce(Force::GenerateSpringForce(*body, anchor, 300.0, 30.0));

        for (auto force : forces)
        {
            body->AddForce(force);
        }

        for (auto torque : torques)
        {
            body->AddTorque(torque);
        }

        body->Update(dt);

        // int minWidthBound = 0, minHeightBound = 0;
        // int maxWidthBound = Graphics::Width(), maxHeightBound = Graphics::Height();
    }

    CheckCollisions();
}

void World::CheckCollisions()
{
    for (int i = 0; i < bodies.size(); i++)
    {
        for (int j = i + 1; j < bodies.size(); j++)
        {
            bodies[i]->isColliding = false;
            bodies[j]->isColliding = false;
            Contact contact;

            if (CollisionDetection::IsColliding(bodies[i], bodies[j], contact))
            {
                bodies[i]->isColliding = true;
                bodies[j]->isColliding = true;
                contact.ResolveCollision();
                // if (debug)
                // {
                //     Graphics::DrawFillCircle(contact.start.x, contact.start.y, 5, 0xFFFF00FF);
                //     Graphics::DrawFillCircle(contact.end.x, contact.end.y, 5, 0xFFFF00FF);
                //     Graphics::DrawLine(contact.a->position.x, contact.a->position.y, contact.a->position.x + contact.normal.x * 15, contact.a->position.y + contact.normal.y * 15, 0xFFFF00FF);
                // }
            }
        }
    }
}