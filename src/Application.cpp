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
    particles.push_back(new Particle(Graphics::Width() / 2, 500, 1.0, 6.0));
    // particles.push_back(new Particle(500, 500, 20.0, 20.0));
    anchor = Vec2(Graphics::Width() / 2, 100);

    liquid.x = 0;
    liquid.y = Graphics::Height() / 2;
    liquid.w = Graphics::Width();
    liquid.h = Graphics::Height() / 2;
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
                particles.push_back(new Particle(x, y, 1.0, 5.0));
            }
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

    // Mutual Gravitation Force
    // Vec2 gravitationForce = Force::GenerateGravitationForce(*particles[0], *particles[1], 1000.0, 5.0, 100.0);
    // particles[0]->AddForce(gravitationForce);
    // particles[1]->AddForce(-gravitationForce);
    for (Particle *particle : particles)
    {
        // Weight
        particle->AddForce(Vec2(0, particle->mass * 9.8f * PIXELS_PER_METRE));
        // Push
        particle->AddForce(pushForce);
        // if (particle->position.y > liquid.y)
        //     // Drag
        //     particle->AddForce(Force::GenerateDragForce(*particle, 0.01));
        // else
        //     // Wind
        //     particle->AddForce(Vec2(2 * PIXELS_PER_METRE, 0));

        // Friction
        // particle->AddForce(Force::GenerateFrictionForce(*particle, 10.0));
        // Drag
        particle->AddForce(Force::GenerateDragForce(*particle, 0.001));
        // Spring
        particle->AddForce(Force::GenerateSpringForce(*particle, anchor, 300.0, 30.0));
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
    // Graphics::DrawFillCircle(200, 200, 40, 0xFFFFFFFF);
    Graphics::DrawLine(anchor.x, anchor.y, particles[0]->position.x, particles[0]->position.y, 0xFFFFFFFF);
    Graphics::DrawFillCircle(anchor.x, anchor.y, 4.0, 0xFF0044FF);
    // Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h, 0xFF6E3713);
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