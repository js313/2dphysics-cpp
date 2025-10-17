#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"

bool Application::IsRunning()
{
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup()
{
    running = Graphics::OpenWindow();
    world = new World(-9.8f);

    const int NUM_BODIES = 10;

    for (int i = 0; i < NUM_BODIES; i++)
    {
        float mass = (i == 0) ? 0 : 1;
        Body *body = new Body(BoxShape(30, 30), Graphics::Width() / 2.0 - i * 40, 100, mass);
        body->SetTexture("./assets/crate.png");
        world->AddBody(body);
    }

    for (int i = 0; i < NUM_BODIES - 1; i++)
    {
        world->AddConstraint(new JointConstraint(world->GetBodies()[i], world->GetBodies()[i + 1], world->GetBodies()[i]->position));
    }
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
            if (event.key.keysym.sym == SDLK_d)
                debug = !debug;
            break;
        case SDL_KEYUP:
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int x = 0, y = 0;
                SDL_GetMouseState(&x, &y);
                Body *stone = new Body(PolygonShape({Vec2(-15, 5), Vec2(-15, 0), Vec2(-5, -10), Vec2(15, -10), Vec2(20, 5), Vec2(10, 10)}), x, y, 10.0);
                world->AddBody(stone);
            }
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                int x = 0, y = 0;
                SDL_GetMouseState(&x, &y);
                Body *basketball = new Body(CircleShape(20.0), x, y, 10.0);
                basketball->SetTexture("./assets/basketball.png");
                world->AddBody(basketball);
            }
            break;
        case SDL_MOUSEMOTION:
            int x = 0, y = 0;
            SDL_GetMouseState(&x, &y);
            // bodies[1]->position.x = x;
            // bodies[1]->position.y = y;
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update()
{
    Graphics::ClearScreen(0xFF0F0721);

    // Wait some time until the reach the target frame time in milliseconds
    static int timePreviousFrame;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0)
        SDL_Delay(timeToWait);

    // Calculate the deltatime in seconds
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016)
        deltaTime = 0.016;

    // Set the time of the current frame to be used in the next one
    timePreviousFrame = SDL_GetTicks();

    // Update world bodies (integration, collision detection, etc.)
    world->Update(deltaTime);
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render()
{
    // Draw background texture
    Graphics::DrawTexture(Graphics::Width() / 2.0, Graphics::Height() / 2.0, Graphics::Width(), Graphics::Height(), 0.0f, bgTexture);

    // Draw all bodies
    for (auto &body : world->GetBodies())
    {
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape *circleShape = (CircleShape *)body->shape;
            if (!debug && body->texture)
            {
                Graphics::DrawTexture(body->position.x, body->position.y, circleShape->radius * 2, circleShape->radius * 2, body->rotation, body->texture);
            }
            else
            {
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFF0000FF);
            }
        }
        if (body->shape->GetType() == BOX)
        {
            BoxShape *boxShape = (BoxShape *)body->shape;
            if (!debug && body->texture)
            {
                Graphics::DrawTexture(body->position.x, body->position.y, boxShape->width, boxShape->height, body->rotation, body->texture);
            }
            else
            {
                Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, 0xFF0000FF);
            }
        }
        if (body->shape->GetType() == POLYGON)
        {
            PolygonShape *polygonShape = (PolygonShape *)body->shape;
            if (!debug && body->texture)
            {
                Graphics::DrawTexture(body->position.x, body->position.y, polygonShape->width, polygonShape->height, body->rotation, body->texture);
            }
            else
            {
                Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->worldVertices, 0xFF0000FF);
            }
        }
    }
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy()
{
    SDL_DestroyTexture(bgTexture);
    delete world;
    Graphics::CloseWindow();
}