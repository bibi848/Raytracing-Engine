#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "onb.h"

class Sphere : public Hittable {
public:
	// Stationnary
	Sphere(const Vec3& static_center, double radius, shared_ptr<Material> mat)
		: center(static_center, Vec3(0,0,0)), radius(std::fmax(0, radius)), mat(mat) 
	{
		auto rvec = Vec3(radius, radius, radius);
		bbox = aabb(static_center - rvec, static_center + rvec);
	}

	// Moving
	Sphere(const Vec3& center1, const Vec3& center2, double radius, shared_ptr<Material> mat) 
		: center(center1, center2 - center1), radius(std::fmax(0,radius)), mat(mat) 
	{
		auto rvec = Vec3(radius, radius, radius);
		aabb box1(center.at(0) - rvec, center.at(0) + rvec);
		aabb box2(center.at(1) - rvec, center.at(1) + rvec);
		bbox = aabb(box1, box2);
	}

	bool hit(const Ray& r, interval ray_t, hit_record& rec) const override {
		Vec3 current_center = center.at(r.time());
		Vec3 oc = current_center - r.origin();
		auto a = r.direction().length_squared();
		auto h = dot(r.direction(), oc);
		auto c = oc.length_squared() - radius * radius;

		auto discriminant = h * h - a * c;
		if (discriminant < 0)
			return false;

		auto sqrtd = std::sqrt(discriminant);

		auto root = (h - sqrtd) / a;
		if (!ray_t.surrounds(root)) {
			root = (h + sqrtd) / a;
			if (!ray_t.surrounds(root))
				return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		Vec3 outward_normal = (rec.p - current_center) / radius;
		rec.set_face_normal(r, outward_normal);
		get_sphere_uv(outward_normal, rec.u, rec.v);
		rec.mat = mat;

		return true;
	}
	
	aabb bounding_box() const override { return bbox; }

	double pdf_value(const Vec3& origin, const Vec3& direction) const override {

		hit_record rec;
		if (!this->hit(Ray(origin, direction), interval(0.001, infinity), rec))
			return 0;

		auto dist_squared = (center.at(0) - origin).length_squared();
		auto cos_theta_max = std::sqrt(1 - radius * radius / dist_squared);
		auto solid_angle = 2 * pi * (1 - cos_theta_max);

		return 1 / solid_angle;
	}

	Vec3 random(const Vec3& origin) const override {
		Vec3 direction = center.at(0) - origin;
		auto distance_squared = direction.length_squared();
		onb uvw(direction);
		return uvw.transform(random_to_sphere(radius, distance_squared));
	}

private:
	Ray center;
	double radius;
	shared_ptr<Material> mat;
	aabb bbox;

	static void get_sphere_uv(const Vec3& p, double& u, double& v) {
		// p: A given point on the unit sphere about (0,0,0)
		// u: Returned value [0,1] of the angle around the Y axis from X = -1
		// v: Returned value [0,1] of the angle from Y = -1 to Y = +1

		auto theta = std::acos(-p.y());
		auto phi = std::atan2(-p.z(), p.x() + pi);

		u = phi / (2 * pi);
		v = theta / pi;
	}

	static Vec3 random_to_sphere(double radius, double distance_squared) {
		auto r1 = random_double();
		auto r2 = random_double();
		auto z = 1 + r2 * (std::sqrt(1 - radius * radius / distance_squared) - 1);

		auto phi = 2 * pi * r1;
		auto x = std::cos(phi) * std::sqrt(1 - z * z);
		auto y = std::sin(phi) * std::sqrt(1 - z * z);

		return Vec3(x, y, z);
	}
};


#endif
