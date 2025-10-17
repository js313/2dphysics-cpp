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
    World *world;

    SDL_Texture *bgTexture;

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