#include "utilities.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include <fstream>


int main() {
    hittable_list world;

    auto R = std::cos(pi / 4);

    auto material_left = make_shared<Lambertian>(colour(0, 0, 1));
    auto material_right = make_shared<Lambertian>(colour(1, 0, 0));

    world.add(make_shared<Sphere>(Vec3(-R, 0, -1), R, material_left));
    world.add(make_shared<Sphere>(Vec3(R, 0, -1), R, material_right));

    Camera camera;

    camera.aspect_ratio = 16.0 / 9.0;
    camera.image_width = 400;
    camera.samples_per_pixel = 20;
    camera.max_depth = 50;
    camera.vfov = 30;

    camera.render(world);

    return 0;
}



