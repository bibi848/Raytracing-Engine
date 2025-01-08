#include "ObjectGroup.h"


// Constructor
Sphere::Sphere(float r, float x, float y, float z) :
    radius(r)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

// Getter
float Sphere::getRadius() const 
{
    return radius;
}

void Sphere::getPosition(float pos[3]) const
{
    pos[0] = position[0];
    pos[1] = position[1];
    pos[2] = position[2];
}

// Setter
void Sphere::setPosition(float x, float y, float z) 
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
}
