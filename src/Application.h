#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include "./Graphics.h"
#include "./Physics/Body.h"

class Application
{
private:
    bool running = false;
    std::vector<Body *> bodies;
    Vec2 pushForce;

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