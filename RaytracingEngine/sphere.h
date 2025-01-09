#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "hittable.h"

class Sphere: public Hittable {
public:
	Sphere(const Vec3& position, float radius)
		: pos(position), rad(radius) {
	}

	bool hit(const Ray& r, float t_min, float t_max) const override;

	Vec3 position() const { return pos; }
	float radius() const { return rad; }

private:
	Vec3 pos;
	float rad;
};


#endif // SPHERE_H
