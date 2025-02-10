#include "utilities.h"

#include "bvh.h"
#include "camera.h"
#include "constant_medium.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "texture.h"

#include <fstream>

void cornell_box() {
    hittable_list world;

    auto red = make_shared<Lambertian>(colour(.65, .05, .05));
    auto white = make_shared<Lambertian>(colour(.73, .73, .73));
    auto green = make_shared<Lambertian>(colour(.12, .45, .15));
    auto light = make_shared<diffuse_light>(colour(15, 15, 15));

    // Cornell box sides
    world.add(make_shared<quad>(Vec3(555, 0, 0), Vec3(0, 0, 555), Vec3(0, 555, 0), green));
    world.add(make_shared<quad>(Vec3(0, 0, 555), Vec3(0, 0, -555), Vec3(0, 555, 0), red));
    world.add(make_shared<quad>(Vec3(0, 555, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(make_shared<quad>(Vec3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 0, -555), white));
    world.add(make_shared<quad>(Vec3(555, 0, 555), Vec3(-555, 0, 0), Vec3(0, 555, 0), white));

    // Light
    world.add(make_shared<quad>(Vec3(213, 554, 227), Vec3(130, 0, 0), Vec3(0, 0, 105), light));

    // Box 1
    shared_ptr<Hittable> box1 = box(Vec3(0, 0, 0), Vec3(165, 330, 165), white);
    box1 = make_shared<Rotate_y>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
    world.add(box1);

    // Box 2
    shared_ptr<Hittable> box2 = box(Vec3(0, 0, 0), Vec3(165, 165, 165), white);
    box2 = make_shared<Rotate_y>(box2, -18);
    box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));
    world.add(box2);

    // Light Sources
    auto empty_material = shared_ptr<Material>();
    quad lights(Vec3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), empty_material);

    world = hittable_list(make_shared<bvh_node>(world));

    Camera cam;

    cam.aspect_ratio = 1.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 10;
    cam.max_depth = 50;
    cam.background = colour(0, 0, 0);

    cam.vfov = 40;
    cam.lookfrom = Vec3(278, 278, -800);
    cam.lookat = Vec3(278, 278, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world, lights);
}

int main() {
    cornell_box();
}



