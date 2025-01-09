#include "sphere.h"
#include "ray.h"
#include <cmath>
#include <iostream>

bool Sphere::hit(const Ray& r, float t_min, float t_max) const {
    Vec3 oc = r.origin() - pos;  
    float a = r.direction().dot(r.direction());  
    float b = 2.0f * oc.dot(r.direction());     
    float c = oc.dot(oc) - rad * rad;            

    float discriminant = b * b - 4.0f * a * c;  // Quadratic discriminant
    if (discriminant > 0)
        return true;
    else
        return false;
}
