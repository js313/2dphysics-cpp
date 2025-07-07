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
    bodies.push_back(new Body(new BoxShape(100, 100), Graphics::Width() / 2, 500, 0.0));
    bodies.push_back(new Body(new BoxShape(Graphics::Width() - 100, 200), Graphics::Width() / 2, Graphics::Height() - 150, 0.0));
    // bodies.push_back(new Body(new CircleShape(50), Graphics::Width() / 2, 500, 0.0));
    // bodies[0]->rotation = 1.4;
    bodies[0]->restitution = 0.1;
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
                // bodies.push_back(new Body(new CircleShape(20.0), x, y, 10.0));
                bodies.push_back(new Body(new PolygonShape({Vec2(-15, 5), Vec2(-15, 0), Vec2(-5, -10), Vec2(15, -10), Vec2(20, 5), Vec2(10, 10)}), x, y, 10.0));
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

    // Mutual Gravitation Force
    // Vec2 gravitationForce = Force::GenerateGravitationForce(*bodies[0], *bodies[1], 1000.0, 5.0, 100.0);
    // bodies[0]->AddForce(gravitationForce);
    // bodies[1]->AddForce(-gravitationForce);
    for (Body *body : bodies)
    {
        // Weight
        body->AddForce(Vec2(0, body->mass * 9.8f * PIXELS_PER_METRE));
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

        // BAD, BAD, VERY BAD!!!
        Graphics::ClearScreen(0xFF056263);

        body->Update(deltaTime);

        // int minWidthBound = 0, minHeightBound = 0;
        // int maxWidthBound = Graphics::Width(), maxHeightBound = Graphics::Height();
    }
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

                if (debug)
                {
                    Graphics::DrawFillCircle(contact.start.x, contact.start.y, 5, 0xFFFF00FF);
                    Graphics::DrawFillCircle(contact.end.x, contact.end.y, 5, 0xFFFF00FF);
                    Graphics::DrawLine(contact.a->position.x, contact.a->position.y, contact.a->position.x + contact.normal.x * 15, contact.a->position.y + contact.normal.y * 15, 0xFFFF00FF);
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render()
{
    // Graphics::ClearScreen(0xFF056263);
    for (Body *body : bodies)
    {
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape *circleShape = (CircleShape *)body->shape;
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, 0xFF00FF00);
        }
        else if (body->shape->GetType() == BOX)
        {
            BoxShape *boxShape = (BoxShape *)body->shape;
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
    for (Body *body : bodies)
    {
        delete body;
    }

    Graphics::CloseWindow();
}