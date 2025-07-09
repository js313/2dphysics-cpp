#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include "./Graphics.h"
#include "./Physics/Body.h"
#include "./Physics/World.h"

class Application
{
private:
    bool debug = false;
    bool running = false;
    Vec2 pushForce;
    World *world;

    // SDL_Rect liquid;

    // Vec2 anchor;

public:
    Application() = default;
    ~Application() = default;
    bool IsRunning();
    void Setup();
    void Input();
    void Update();
    void Render();
    void Destroy();
};

#endif