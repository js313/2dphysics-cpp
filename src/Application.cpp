#include "Application.h"
#include "./Physics/Constants.h"
#include <iostream>

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
    for (int i = 0; i < 2; i++)
    {
        particles.push_back(new Particle(50, 100, 1.0, 4.0));
        particles.push_back(new Particle(100, 100, 3.0, 10.0));
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
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update()
{
    // Cap FPS
    static int lastFrameTime;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - lastFrameTime);
    if (timeToWait > 0)
        SDL_Delay(timeToWait);

    // To achieve framerate independent movement
    // Calculate this only after applying delay(capping FPS), or else the lastFrameTime won't be correct.
    // it would take the frame that we decided not to update anything on as the last frame, we want the frame
    // that made changes to the screen
    int currentFrameTime = SDL_GetTicks();
    float deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f;

    lastFrameTime = currentFrameTime;

    for (Particle *particle : particles)
    {
        // Wind
        particle->AddForce(Vec2(2 * PIXELS_PER_METRE, 0));
        // Weight
        particle->AddForce(Vec2(0, particle->mass * 9.8f * PIXELS_PER_METRE));
        particle->Integrate(deltaTime);

        int minWidthBound = 0, minHeightBound = 0;
        int maxWidthBound = Graphics::Width(), maxHeightBound = Graphics::Height();
        if (particle->position.x - particle->radius < minWidthBound || particle->position.x + particle->radius > maxWidthBound)
        {
            particle->velocity.x *= -1;
            if (particle->position.x + particle->radius < minWidthBound)
                particle->position.x = particle->radius;
            else
                particle->position.x = maxWidthBound - particle->radius;
        }
        if (particle->position.y - particle->radius < minHeightBound || particle->position.y + particle->radius > maxHeightBound)
        {
            particle->velocity.y *= -1;
            if (particle->position.y + particle->radius < minWidthBound)
                particle->position.y = particle->radius;
            else
                particle->position.y = maxHeightBound - particle->radius;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render()
{
    Graphics::ClearScreen(0xFF056263);
    // Graphics::DrawFillCircle(200, 200, 40, 0xFFFFFFFF);
    for (Particle *particle : particles)
    {

        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->mass * 4, 0xFFFFFFFF);
    }
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy()
{
    for (Particle *particle : particles)
    {
        delete particle;
    }

    Graphics::CloseWindow();
}