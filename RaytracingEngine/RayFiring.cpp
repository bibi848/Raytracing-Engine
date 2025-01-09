#include <iostream>
#include "FunctionGroup.h"
#include "vec3.h"
#include "hittable.h"
#include "sphere.h"
#include "ray.h"

std::vector<std::vector<std::vector<int>>> RayFiring(std::vector<std::vector<std::vector<int>>> PixelArray,
    Vec3 PixelPos, const int ImageWidth, const int ImageHeight, Vec3 CameraPosition, Vec3 CameraNormal,
    const float MaxRayDist, Vec3 SpherePosition, float radius)
{   
    float original_y = PixelPos.y;
    Sphere sphere(SpherePosition, radius);

    for (int y = 0; y < ImageHeight; ++y)
    {
        for (int x = 0; x < ImageWidth; ++x)
        {
            Vec3 ray_direction = CameraNormal;
            Ray r(PixelPos, ray_direction);
            bool intersectionTest = sphere.hit(r, 0.001f, MaxRayDist);
            
            
            PixelPos.y = PixelPos.y - 0.001f;

            if (intersectionTest) {
                PixelArray[y][x][0] = 0;
                PixelArray[y][x][1] = 0;
                PixelArray[y][x][2] = 0;
            }
            else {
                PixelArray[y][x][0] = 255;
                PixelArray[y][x][1] = 255;
                PixelArray[y][x][2] = 255;
            }
        }
        PixelPos.z = PixelPos.z - 0.001f;
        PixelPos.y = original_y;
    }
    
    return PixelArray;
}