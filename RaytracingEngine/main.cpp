#include <vector>
#include <iostream>

#include "FunctionGroup.h"
#include "ray.h"
#include "vec3.h"
#include "sphere.h"
#include "camera.h"

const int ImageWidth = 1000;
const int ImageHeight = 800;
std::vector<std::vector<std::vector<int>>> PixelArray(ImageHeight, std::vector<std::vector<int>>(ImageWidth, std::vector<int>(3)));
Vec3 TopLeftPixelPos(-1.0f, static_cast<float>(ImageWidth) / 2000.0f, static_cast<float>(ImageWidth) / 2000.0f);

bool SaveImage = false;
const char* filename = "C:/Users/oscar/Documents/C++/Raytracing-Engine/Images/First Sphere Test Image.png";

const float MaxRayDist = 1000.0f;

bool checkHit(const Ray& r, const std::vector<Hittable*>& world) {
    for (const auto& obj : world) {
        if (obj->hit(r, 0.0f, MaxRayDist)) {
            return true;
        }
    }
    return false;
}

int main() {

    Vec3 sphere_position(0, 0, 0);
    float sphere_radius = 0.2f;
    Vec3 camera_position(-1, 0, 0);
    Vec3 camera_direction(1, 0, 0);

    Sphere sphere(sphere_position, sphere_radius);
    Camera camera(camera_position, camera_direction, ImageWidth, ImageHeight);


    PixelArray = RayFiring(PixelArray, TopLeftPixelPos, ImageWidth, ImageHeight, camera.position(), camera.direction(), 
        MaxRayDist, sphere.position(), sphere.radius());

    RenderImage(ImageWidth, ImageHeight, PixelArray, SaveImage, filename);

    return 0; 
}


