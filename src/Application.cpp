#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"

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

    Body *bigBox = new Body(new BoxShape(100, 100), Graphics::Width() / 2, 500, 0.0);
    bigBox->restitution = 0.1;
    bigBox->rotation = 0.1;
    bigBox->SetTexture("./assets/crate.png");

    Body *floor = new Body(new BoxShape(Graphics::Width() - 100, 200), Graphics::Width() / 2, Graphics::Height() - 150, 0.0);

    world->AddBody(bigBox);
    world->AddBody(floor);
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
            if (event.key.keysym.sym == SDLK_d)
                debug = !debug;
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
                Body *stone = new Body(new PolygonShape({Vec2(-15, 5), Vec2(-15, 0), Vec2(-5, -10), Vec2(15, -10), Vec2(20, 5), Vec2(10, 10)}), x, y, 10.0);
                world->AddBody(stone);
            }
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                int x = 0, y = 0;
                SDL_GetMouseState(&x, &y);
                Body *basketball = new Body(new CircleShape(20.0), x, y, 10.0);
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

    world->Update(deltaTime);
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render()
{
    Graphics::ClearScreen(0xFF056263);
    for (Body *body : world->GetBodies())
    {
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape *circleShape = (CircleShape *)body->shape;
            if (!debug && body->texture)
                Graphics::DrawTexture(body->position.x, body->position.y, 2 * circleShape->radius, 2 * circleShape->radius, body->rotation, body->texture);
            else
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFF00FF00);
        }
        else if (body->shape->GetType() == BOX)
        {
            BoxShape *boxShape = (BoxShape *)body->shape;
            if (!debug && body->texture)
                Graphics::DrawTexture(body->position.x, body->position.y, boxShape->width, boxShape->height, body->rotation, body->texture);
            else
                Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->globalVertices, 0xFF00FF00);
        }
        else if (body->shape->GetType() == POLYGON)
        {
            PolygonShape *polygonShape = (PolygonShape *)body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->globalVertices, 0xFF00FF00);
        }
    }
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy()
{
    delete world;
    Graphics::CloseWindow();
}