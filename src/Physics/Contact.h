#ifndef CONTACT_H
#define CONTACT_H

#include "./Body.h"

struct Contact
{
    // These should've been const I don't care!!
    // const Body *a; // stops you from changing properties of body
    // const Body *b; // `Body const *a` this stops you from reassgning the pointer

    Body *a;
    Body *b;

    Vec2 start;
    Vec2 end;

    Vec2 normal;
    float depth;

    Contact() = default;
    ~Contact() = default;

    void ResolvePenetration();
};

#endif