#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include "./Graphics.h"
#include "./Physics/Particle.h"

class Application
{
private:
    bool running = false;
    std::vector<Particle *> particles;
    Vec2 pushForce;

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