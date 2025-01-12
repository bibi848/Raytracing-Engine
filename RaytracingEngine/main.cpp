#include "utilities.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include <fstream>


int main() {
    hittable_list world;

    world.add(make_shared<Sphere>(Vec3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Vec3(0, -100.5, -1), 100));

    Camera camera;

    camera.aspect_ratio = 16.0 / 9.0;
    camera.image_width = 400;

    camera.render(world);

    return 0;
}



