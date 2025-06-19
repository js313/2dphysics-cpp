#include "./Contact.h"

void Contact::ResolvePenetration()
{
    if (a->IsStatic() && b->IsStatic())
        return;
    float da = (depth * (a->invMass)) / (a->invMass + b->invMass);
    float db = (depth * (b->invMass)) / (a->invMass + b->invMass);

    a->position -= normal * da;
    b->position += normal * db;
}