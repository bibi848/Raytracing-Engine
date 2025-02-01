#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class Material {
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& r_in, const hit_record& rec, colour& attenuation, Ray& scattered) const {
		return false;
	}
};

class Lambertian : public Material {
public:
	Lambertian(const colour& albedo) : albedo(albedo) {}

	bool scatter(const Ray& r_in, const hit_record& rec, colour& attenuation, Ray& scattered) const override {
		auto scatter_direction = rec.normal + random_unit_vector();

		if (scatter_direction.near_zero()) {
			scatter_direction = rec.normal;
		}

		scattered = Ray(rec.p, scatter_direction, r_in.time());
		attenuation = albedo;
		return true;
	}
private:
	colour albedo;
};

class Metal : public Material {
public:
	Metal(const colour& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(const Ray& r_in, const hit_record& rec, colour& attenuation, Ray& scattered) const override {
		Vec3 reflected = reflect(r_in.direction(), rec.normal);
		reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
		scattered = Ray(rec.p, reflected, r_in.time());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	colour albedo;
	double fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(double refraction_index) : refraction_index(refraction_index) {}

	bool scatter(const Ray& r_in, const hit_record& rec, colour& attenuation, Ray& scattered) const override {
		attenuation = colour(1.0, 1.0, 1.0);
		double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

		Vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = ri * sin_theta > 1.0;
		Vec3 direction;

		if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
			direction = reflect(unit_direction, rec.normal);
		}
		else {
			direction = refract(unit_direction, rec.normal, ri);
		}
		scattered = Ray(rec.p, direction, r_in.time());
		return true;
	}

private:
	double refraction_index;

	static double reflectance(double cosine, double refraction_index) {
		auto r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};
#endif

