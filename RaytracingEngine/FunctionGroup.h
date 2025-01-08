#ifndef FUNCTIONGROUP_H
#define FUNCTIONGROUP_H

#include <vector>


// Function declaration
int RenderImage(const int WIDTH, const int HEIGHT, std::vector<std::vector<std::vector<int>>>& PixelArray);
bool SphereIntersect(float rayOrigin[3], float rayDirection[3], float SpherePosition[3], float radius, float& t0, float& t1);

#endif // FUNCTIONGROUP_H
