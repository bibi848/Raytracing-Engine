#ifndef RAY_H
#define RAY_H

#include "vec3.h" 

class Ray {
public:
    Ray() {}
    Ray(const Vec3& origin, const Vec3& direction)
        : orig(origin), dir(direction) {
    }

    Vec3 origin() const { return orig; }
    Vec3 direction() const { return dir; }
    Vec3 point_at_parameter(float t) const {
        return orig + dir.scMult(t);
    }

private:
    Vec3 orig; 
    Vec3 dir;  
};

#endif // RAY_H
