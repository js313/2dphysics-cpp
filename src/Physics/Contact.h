#ifndef CONTACT_H
#define CONTACT_H

#include "./Body.h"

struct Contact
{
    const Body *a; // stops you from changing properties of body
    const Body *b; // `Body const *a` this stops you from reassgning the pointer

    Vec2 start;
    Vec2 end;

    Vec2 normal;
    float depth;

    Contact() = default;
    ~Contact() = default;
};

#endif