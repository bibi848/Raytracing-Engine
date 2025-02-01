#ifndef RAY_H
#define RAY_H

#include "vec3.h" 

class Ray {
public:
    Ray() {}

    Ray(const Vec3& origin, const Vec3& direction, double time)
        : orig(origin), dir(direction), tm(time) {}

    Ray(const Vec3& origin, const Vec3& direction)
        : Ray(origin, direction, 0) {}

    Vec3 origin() const { return orig; }
    Vec3 direction() const { return dir; }

    double time() const { return tm; }

    Vec3 at(double t) const {
        return orig + t * dir;
    }

private:
    Vec3 orig; 
    Vec3 dir;  
    double tm;
};

#endif 
