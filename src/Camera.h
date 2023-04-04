#ifndef MARKER3
#define MARKER3
#include "framework.h"
#include "Intersectable.h"

class Camera {
	Vec3f eye, lookat, right, up;
	float fov;
public:
	void set(Vec3f _eye, Vec3f _lookat, Vec3f vup, float _fov);
	Ray getRay(int X, int Y);

};
#endif