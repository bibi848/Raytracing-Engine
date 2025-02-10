#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "onb.h"
#include "texture.h"

class Material {
public:
	virtual ~Material() = default;

	virtual colour emitted(const Ray& r_in, const hit_record& rec, double u, double v, const Vec3& p) const {
		return colour(0, 0, 0);
	}

	virtual bool scatter(const Ray& r_in, const hit_record& rec, colour& attenuation, Ray& scattered, double& pdf) const {
		return false;
	}

	virtual double scattering_pdf(const Ray& r_in, const hit_record& rec, const Ray& scattered)
		const {
		return 0;
	}
};

class Lambertian : public Material {
public:
	Lambertian(const colour& albedo) : tex(make_shared<solid_colour>(albedo)) {}
	Lambertian(shared_ptr<Texture> tex) : tex(tex) {}

	bool scatter(const Ray& r_in, const hit_record& rec, colour& attenuation, Ray& scattered, double& pdf) const override {
		onb uvw(rec.normal);
		auto scatter_direction = uvw.transform(random_cosine_direction());

		scattered = Ray(rec.p, unit_vector(scatter_direction), r_in.time());
		attenuation = tex->value(rec.u, rec.v, rec.p);
		pdf = dot(uvw.w(), scattered.direction()) / pi;
		return true;
	}

	double scattering_pdf(const Ray& r_in, const hit_record& rec, const Ray& scattered)
		const override {
		auto cos_theta = dot(rec.normal, unit_vector(scattered.direction()));
		return cos_theta < 0 ? 0 : cos_theta / pi;
	}

private:
	shared_ptr<Texture> tex;
};

class Metal : public Material {
public:
	Metal(const colour& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(const Ray& r_in, const hit_record& rec, colour& attenuation, Ray& scattered, double& pdf) const override {
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

	bool scatter(const Ray& r_in, const hit_record& rec, colour& attenuation, Ray& scattered, double& pdf) const override {
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

class diffuse_light : public Material {
public:
	diffuse_light(shared_ptr<Texture> tex) : tex(tex) {}
	diffuse_light(const colour& emit) : tex(make_shared<solid_colour>(emit)) {}

	colour emitted(const Ray& r_in, const hit_record& rec, double u, double v, const Vec3& p) const override {
		if (!rec.front_face)
			return colour(0, 0, 0);
		return tex->value(u, v, p);
	}

private:
	shared_ptr<Texture> tex;
};

class isotropic : public Material {
public:
	isotropic(const colour& albedo) : tex(make_shared<solid_colour>(albedo)) {}
	isotropic(shared_ptr<Texture> tex) : tex(tex) {}

	bool scatter(const Ray& r_in, const hit_record& rec, colour& attenuation, Ray& scattered, double& pdf)
		const override {
		scattered = Ray(rec.p, random_unit_vector(), r_in.time());
		attenuation = tex->value(rec.u, rec.v, rec.p);
		pdf = 1 / (4 * pi);
		return true;
	}

	double scattering_pdf(const Ray& r_in, const hit_record& rec, const Ray& scattered) const override {
		return 1 / (4 * pi);
	}

private:
	shared_ptr<Texture> tex;
};
#endif

