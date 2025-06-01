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

    Vec2 anchor;
    int particleCount = 15;
    float k = 300;
    float restLength = 20;

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