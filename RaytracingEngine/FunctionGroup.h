#ifndef FUNCTIONGROUP_H
#define FUNCTIONGROUP_H

#include <vector>
#include "vec3.h"


// Function declaration
int RenderImage(const int ImageWidth, const int ImageHeight, std::vector<std::vector<std::vector<int>>>& PixelArray, bool SaveImage, const char* filename);
bool SphereIntersect(float rayOrigin[3], float rayDirection[3], float SpherePosition[3], float radius);
std::vector<std::vector<std::vector<int>>> RayFiring(std::vector<std::vector<std::vector<int>>> PixelArray,
	Vec3 TopLeftPixelPos, const int ImageWidth, const int ImageHeight, Vec3 CameraPosition, Vec3 CameraNormal,
	const float MaxRayDist, Vec3 SpherePosition, float radius);

#endif // FUNCTIONGROUP_H
