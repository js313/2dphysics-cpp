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
    bodies.push_back(new Body(new CircleShape(200), Graphics::Width() / 2, 100, 1.0));
    bodies.push_back(new Body(new CircleShape(100), Graphics::Width() / 2, 500, 1.0));
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
                bodies.push_back(new Body(new CircleShape(5.0), x, y, 1.0));
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
        //     // Drag
        //     body->AddForce(Force::GenerateDragForce(*body, 0.01));
        // else
        // Wind
        body->AddForce(Vec2(10 * PIXELS_PER_METRE, 0));

        // Friction
        // body->AddForce(Force::GenerateFrictionForce(*body, 10.0));
        // Drag
        // body->AddForce(Force::GenerateDragForce(*body, 0.003));
        // Torque
        // body->AddTorque(200.0);
        // Spring
        // body->AddForce(Force::GenerateSpringForce(*body, anchor, 300.0, 30.0));
        body->Update(deltaTime);

        int minWidthBound = 0, minHeightBound = 0;
        int maxWidthBound = Graphics::Width(), maxHeightBound = Graphics::Height();

        if (body->shape->GetType() == CIRCLE) // Need this as type casting shape to circleShape inside
        {
            CircleShape *circleShape = (CircleShape *)body->shape;
            if (body->position.x - circleShape->radius < minWidthBound || body->position.x + circleShape->radius > maxWidthBound)
            {
                body->velocity.x *= -0.9;
                if (body->position.x - circleShape->radius < minWidthBound)
                    body->position.x = circleShape->radius;
                else
                    body->position.x = maxWidthBound - circleShape->radius;
            }
            if (body->position.y - circleShape->radius < minHeightBound || body->position.y + circleShape->radius > maxHeightBound)
            {
                body->velocity.y *= -0.9;
                if (body->position.y - circleShape->radius < minWidthBound)
                    body->position.y = circleShape->radius;
                else
                    body->position.y = maxHeightBound - circleShape->radius;
            }
        }
    }
    for (int i = 0; i < bodies.size(); i++)
    {
        for (int j = i + 1; j < bodies.size(); j++)
        {
            bodies[i]->isColliding = false;
            bodies[j]->isColliding = false;

            if (CollisionDetection::IsColliding(bodies[i], bodies[j]))
            {
                bodies[i]->isColliding = true;
                bodies[j]->isColliding = true;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render()
{
    Graphics::ClearScreen(0xFF056263);
    for (Body *body : bodies)
    {
        Uint32 color = body->isColliding ? 0xFF0000FF : 0xFFFFFFFF;
        if (body->shape->GetType() == CIRCLE)
        {
            CircleShape *circleShape = (CircleShape *)body->shape;
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, color);
        }
        else if (body->shape->GetType() == BOX)
        {
            BoxShape *boxShape = (BoxShape *)body->shape;
            Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->globalVertices, color);
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