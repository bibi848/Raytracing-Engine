#ifndef OBJECTGROUP_H
#define OBJECTGROUP_H

#include <iostream>
#include <numbers>


class Sphere {
private:
    float radius;
    float position[3];

public:
    // Constructor
    Sphere(float r, float x = 0, float y = 0, float z = 0);

    // Getter
    float getRadius() const;
    void getPosition(float pos[3]) const;

    // Setter
    void setPosition(float x, float y, float z);
};

#endif
