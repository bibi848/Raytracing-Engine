#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class Hittable {
public:
    virtual bool hit(const Ray& r, float t_min, float t_max) const = 0; // Virtual method

    virtual ~Hittable() = default; // Cleanup of derived classes
};

#endif // HITTABLE_H

