#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Body.h"

class Contraint
{
public:
    Body *a;
    Body *b;

    void Solve();
};

#endif