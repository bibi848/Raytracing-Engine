#ifndef PDF_H
#define PDF_H

#include "hittable_list.h"
#include "onb.h"


class pdf {
public:
    virtual ~pdf() {}

    virtual double value(const Vec3& direction) const = 0;
    virtual Vec3 generate() const = 0;
};

class sphere_pdf : public pdf {
public:
    sphere_pdf() {}

    double value(const Vec3& direction) const override {
        return 1 / (4 * pi);
    }

    Vec3 generate() const override {
        return random_unit_vector();
    }
};

class cosine_pdf : public pdf {
public:
    cosine_pdf(const Vec3& w) : uvw(w) {}

    double value(const Vec3& direction) const override {
        auto cosine_theta = dot(unit_vector(direction), uvw.w());
        return std::fmax(0, cosine_theta / pi);
    }

    Vec3 generate() const override {
        return uvw.transform(random_cosine_direction());
    }

private:
    onb uvw;
};

class hittable_pdf : public pdf {
public:
    hittable_pdf(const Hittable& objects, const Vec3& origin)
        : objects(objects), origin(origin)
    {
    }

    double value(const Vec3& direction) const override {
        return objects.pdf_value(origin, direction);
    }

    Vec3 generate() const override {
        return objects.random(origin);
    }

private:
    const Hittable& objects;
    Vec3 origin;
};
#endif