#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"

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
    particles.push_back(new Particle(Graphics::Width() / 2, 120, 1.0, 6.0));
    for (int i = 1; i < particleCount; i++)
    {
        particles.push_back(new Particle(Graphics::Width() / 2, particles[i - 1]->position.y + restLength, 1.0, 6.0));
    }
    anchor = Vec2(Graphics::Width() / 2, 100);
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
            if (event.key.keysym.sym == SDLK_UP)
                pushForce.y = -50 * PIXELS_PER_METRE;
            if (event.key.keysym.sym == SDLK_DOWN)
                pushForce.y = 50 * PIXELS_PER_METRE;
            if (event.key.keysym.sym == SDLK_LEFT)
                pushForce.x = -50 * PIXELS_PER_METRE;
            if (event.key.keysym.sym == SDLK_RIGHT)
                pushForce.x = 50 * PIXELS_PER_METRE;
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_UP)
                pushForce.y = 0;
            if (event.key.keysym.sym == SDLK_DOWN)
                pushForce.y = 0;
            if (event.key.keysym.sym == SDLK_LEFT)
                pushForce.x = 0;
            if (event.key.keysym.sym == SDLK_RIGHT)
                pushForce.x = 0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int x = 0, y = 0;
                SDL_GetMouseState(&x, &y);
                Vec2 d = Vec2(x, y) - particles[particleCount - 1]->position;
                Vec2 pushDirection = d.UnitVector();
                pushForce = pushDirection * d.Magnitude() * PIXELS_PER_METRE;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
                pushForce = Vec2(0, 0);
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
    if (deltaTime > 0.016) // for 60FPS each frame is at max 0.016s
        deltaTime = 0.016;

    lastFrameTime = currentFrameTime;

    particles[0]->AddForce(Force::GenerateSpringForce(*particles[0], anchor, restLength, k));
    for (int i = 1; i < particleCount; i++)
    {
        Particle *prevParticle = particles[i - 1];
        Particle *currParticle = particles[i];

        // Spring
        Vec2 springForce = Force::GenerateSpringForce(*currParticle, *prevParticle, restLength, k);
        currParticle->AddForce(springForce);
        prevParticle->AddForce(-springForce);

        // Push
        if (i == particleCount - 1)
            currParticle->AddForce(pushForce);
    }

    for (Particle *particle : particles)
    {
        // Weight
        particle->AddForce(Vec2(0, particle->mass * 9.8f * PIXELS_PER_METRE));
        // Drag
        particle->AddForce(Force::GenerateDragForce(*particle, 0.001));

        particle->Integrate(deltaTime);

        int minWidthBound = 0, minHeightBound = 0;
        int maxWidthBound = Graphics::Width(), maxHeightBound = Graphics::Height();
        if (particle->position.x - particle->radius < minWidthBound || particle->position.x + particle->radius > maxWidthBound)
        {
            particle->velocity.x *= -0.95;
            if (particle->position.x - particle->radius < minWidthBound)
                particle->position.x = particle->radius;
            else
                particle->position.x = maxWidthBound - particle->radius;
        }
        if (particle->position.y - particle->radius < minHeightBound || particle->position.y + particle->radius > maxHeightBound)
        {
            particle->velocity.y *= -0.95;
            if (particle->position.y - particle->radius < minWidthBound)
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
    Graphics::DrawLine(anchor.x, anchor.y, particles[0]->position.x, particles[0]->position.y, 0xFFFFFFFF);
    Graphics::DrawFillCircle(anchor.x, anchor.y, 4.0, 0xFF0044FF);

    for (int i = 1; i < particleCount; i++)
    {
        Particle *prevParticle = particles[i - 1];
        Particle *currParticle = particles[i];

        Graphics::DrawLine(prevParticle->position.x, prevParticle->position.y, currParticle->position.x, currParticle->position.y, 0xFFFFFFFF);
    }
    for (Particle *particle : particles)
    {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
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