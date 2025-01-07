
#include "FunctionGroup.h"

const int ImageWidth = 1000;
const int ImageHeight = 800;

int CameraPosition[3] = { -1, 0, 0 };
int CameraNormal[3] = { 1, 0, 0 };

const int MaxRayLen = 100;


int main() {
    
    RenderImage(ImageWidth, ImageHeight);
    return 0;
}


