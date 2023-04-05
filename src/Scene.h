#ifndef MARKER2
#define MARKER2
#include "framework.h"
#include "Intersectable.h"
#include "Camera.h"
#include "common.h"
#include "MaterialGLobal.h"
#include "ModelBase.h"
#include "Light.h"


class Scene {
public:

	void build();
	void render(std::vector<Vec4f>& image);
	//Hit firstIntersect(Ray ray);
	//bool shadowIntersect(Ray ray);
	//vec3 trace(Ray ray, int depth);
	std::vector<ModelBase *> objects;
	std::vector<Light *> lights;
	Vec3f CastRay(const Vec3f& origin, const Vec3f& direction, size_t currentDepth) const;
	bool SceneIntersect(const Vec3f& origin, const Vec3f direction,
		Vec3f& hit, Vec3f& normal, MaterialGlobal& material) const;
	inline Vec3f Reflect(const Vec3f& l, const Vec3f& n) const;
	Vec3f Refract(const Vec3f& I, const Vec3f& N, const float etat, float etai=1.0f) const;


private:
	Vec3f mOrbitCameraParameter; // (x,y,z) => (r, yaw, pitch)
	Vec3f mCameraPosition;
	Vec3f mCameraForward;
	Vec3f mCameraRight;
	Vec3f mCameraUp;

	Camera camera;
	int maxDepth = 3;
	float fov;
};
#endif