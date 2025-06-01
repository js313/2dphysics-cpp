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
    particles.push_back(new Particle(100, 100, 1.0, 6.0));
    particles.push_back(new Particle(200, 100, 1.0, 6.0));
    particles.push_back(new Particle(200, 200, 1.0, 6.0));
    particles.push_back(new Particle(100, 200, 1.0, 6.0));
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

    // Spring
    Vec2 forceAB = Force::GenerateSpringForce(*particles[0], *particles[1], 100, k);
    Vec2 forceBC = Force::GenerateSpringForce(*particles[1], *particles[2], 100, k);
    Vec2 forceCD = Force::GenerateSpringForce(*particles[2], *particles[3], 100, k);
    Vec2 forceDA = Force::GenerateSpringForce(*particles[3], *particles[0], 100, k);
    Vec2 forceAC = Force::GenerateSpringForce(*particles[0], *particles[2], 141, k);
    Vec2 forceBD = Force::GenerateSpringForce(*particles[1], *particles[3], 141, k);
    particles[0]->AddForce(forceAB - forceDA + forceAC);
    particles[1]->AddForce(-forceAB + forceBC + forceBD);
    particles[2]->AddForce(-forceBC - forceAC + forceCD);
    particles[3]->AddForce(forceDA - forceBD - forceCD);

    for (Particle *particle : particles)
    {
        // Weight
        particle->AddForce(Vec2(0, particle->mass * 9.8f * PIXELS_PER_METRE));
        // Drag
        particle->AddForce(Force::GenerateDragForce(*particle, 0.003));
        // Push
        particle->AddForce(pushForce);
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
    Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, particles[1]->position.x, particles[1]->position.y, 0xFFFFFFFF);
    Graphics::DrawLine(particles[1]->position.x, particles[1]->position.y, particles[2]->position.x, particles[2]->position.y, 0xFFFFFFFF);
    Graphics::DrawLine(particles[2]->position.x, particles[2]->position.y, particles[3]->position.x, particles[3]->position.y, 0xFFFFFFFF);
    Graphics::DrawLine(particles[3]->position.x, particles[3]->position.y, particles[0]->position.x, particles[0]->position.y, 0xFFFFFFFF);
    Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, particles[2]->position.x, particles[2]->position.y, 0xFFFFFFFF);
    Graphics::DrawLine(particles[1]->position.x, particles[1]->position.y, particles[3]->position.x, particles[3]->position.y, 0xFFFFFFFF);

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