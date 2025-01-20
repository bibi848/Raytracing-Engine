#include "utilities.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include <fstream>


int main() {
    hittable_list world;

    auto material_ground = make_shared<Lambertian>(colour(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(colour(0.1, 0.2, 0.5));
    auto material_left = make_shared<Dielectric>(1.50);
    auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);
    auto material_right = make_shared<Metal>(colour(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(Vec3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Vec3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<Sphere>(Vec3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(Vec3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<Sphere>(Vec3(1.0, 0.0, -1.0), 0.5, material_right));

    Camera camera;

    camera.aspect_ratio = 16.0 / 9.0;
    camera.image_width = 800;
    camera.samples_per_pixel = 100;
    camera.max_depth = 50;

    camera.vfov = 30;
    camera.lookfrom = Vec3(-2, 2, 1);
    camera.lookat = Vec3(0, 0, -1);
    camera.vup = Vec3(0, 1, 0);

    camera.render(world);

    return 0;
}



