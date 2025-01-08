
#include "FunctionGroup.h"
#include "ObjectGroup.h"
#include <vector>

const int ImageWidth = 1000;
const int ImageHeight = 800;
std::vector<std::vector<std::vector<int>>> PixelArray(ImageHeight, std::vector<std::vector<int>>(ImageWidth, std::vector<int>(3)));
float TopLeftPixelPos[2] = { (-ImageWidth / 2000), (ImageHeight / 2000) }; // Each pixel is 1mm

float CameraPosition[3] = { -1.0, 0.0, 0.0 };
float CameraNormal[3] = { 1.0, 0.0, 0.0 };

const int MaxRayDist = 100;

int main() {

    Sphere sphere(0.2f, 0.0f, 0.0f, 0.0f);
    float SpherePosition[3];
    sphere.getPosition(SpherePosition);

    for (int y = 0; y < ImageHeight; ++y)
    {
        for (int x = 0; x < ImageWidth; ++x)
        {
            if (y % 2 == 0) {
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
    }

    RenderImage(ImageWidth, ImageHeight, PixelArray);
    return 0; 
}


