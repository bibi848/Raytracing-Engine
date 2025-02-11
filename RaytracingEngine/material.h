#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "pdf.h"
#include "texture.h"

class scatter_record {
public:
	colour attenuation;
	shared_ptr<pdf> pdf_ptr;
	bool skip_pdf;
	Ray skip_pdf_ray;
};

class Material {
public:
	virtual ~Material() = default;

	virtual colour emitted(const Ray& r_in, const hit_record& rec, double u, double v, const Vec3& p) const {
		return colour(0, 0, 0);
	}

	virtual bool scatter(const Ray& r_in, const hit_record& rec, scatter_record& srec) const {
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

	bool scatter(const Ray& r_in, const hit_record& rec, scatter_record& srec) const override {
		srec.attenuation = tex->value(rec.u, rec.v, rec.p);
		srec.pdf_ptr = make_shared<cosine_pdf>(rec.normal);
		srec.skip_pdf = false;
		return true;
	}

	double scattering_pdf(const Ray& r_in, const hit_record& rec, const Ray& scattered) const override {
		auto cos_theta = dot(rec.normal, unit_vector(scattered.direction()));
		return cos_theta < 0 ? 0 : cos_theta / pi;
	}

private:
	shared_ptr<Texture> tex;
};

class Metal : public Material {
public:
	Metal(const colour& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(const Ray& r_in, const hit_record& rec, scatter_record& srec) const override {
		Vec3 reflected = reflect(r_in.direction(), rec.normal);
		reflected = unit_vector(reflected) + (fuzz * random_unit_vector());

		srec.attenuation = albedo;
		srec.pdf_ptr = nullptr;
		srec.skip_pdf = true;
		srec.skip_pdf_ray = Ray(rec.p, reflected, r_in.time());

		return true;
	}

private:
	colour albedo;
	double fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(double refraction_index) : refraction_index(refraction_index) {}

	bool scatter(const Ray& r_in, const hit_record& rec, scatter_record& srec) const override {
		srec.attenuation = colour(1.0, 1.0, 1.0);
		srec.pdf_ptr = nullptr;
		srec.skip_pdf = true;

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
		srec.skip_pdf_ray = Ray(rec.p, direction, r_in.time());
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

	bool scatter(const Ray& r_in, const hit_record& rec, scatter_record& srec) const override {
		srec.attenuation = tex->value(rec.u, rec.v, rec.p);
		srec.pdf_ptr = make_shared<sphere_pdf>();
		srec.skip_pdf = false;
		return true;
	}

	double scattering_pdf(const Ray& r_in, const hit_record& rec, const Ray& scattered) const override {
		return 1 / (4 * pi);
	}

private:
	shared_ptr<Texture> tex;
};
#endif

