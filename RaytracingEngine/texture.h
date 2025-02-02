#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.h"

class Texture {
public:
    virtual ~Texture() = default;

    virtual colour value(double u, double v, const Vec3& p) const = 0;
};

class solid_colour : public Texture {
public:
    solid_colour(const colour& albedo) : albedo(albedo) {}

    solid_colour(double red, double green, double blue) : solid_colour(colour(red, green, blue)) {}

    colour value(double u, double v, const Vec3& p) const override {
        return albedo;
    }

private:
    colour albedo;
};

class checker_texture : public Texture {
public:
    checker_texture(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd)
        : inv_scale(1.0 / scale), even(even), odd(odd) {
    }

    checker_texture(double scale, const colour& c1, const colour& c2)
        : checker_texture(scale, make_shared<solid_colour>(c1), make_shared<solid_colour>(c2)) {
    }

    colour value(double u, double v, const Vec3& p) const override {
        auto xInteger = int(std::floor(inv_scale * p.x()));
        auto yInteger = int(std::floor(inv_scale * p.y()));
        auto zInteger = int(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double inv_scale;
    shared_ptr<Texture> even;
    shared_ptr<Texture> odd;
};

class noise_texture : public Texture {
public:
    noise_texture(double scale) : scale(scale) {}

    colour value(double u, double v, const Vec3& p) const override {
        return colour(1, 1, 1) * noise.turb(p, 7);
    }
private:
    perlin noise;
    double scale;
};

#endif