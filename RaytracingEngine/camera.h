#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

class Camera {
public:
	Camera(const Vec3& position, const Vec3& direction, int imageWidth, int imageHeight)
		: pos(position), dir(direction), width(imageWidth), height(imageHeight) {
	}

	Vec3 position() const { return pos; }
	Vec3 direction() const { return dir; }

	int imageResolution() const {
		return width * height;
	}
	float aspectRatio() const {
		return static_cast<float>(width) / height;
	}


private:
	Vec3 pos;
	Vec3 dir;
	int width;
	int height;

};
#endif // CAMERA_H