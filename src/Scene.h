#ifndef MARKER2
#define MARKER2
#include "framework.h"
#include "Intersectable.h"
#include "Camera.h"
#include "common.h"


struct Light {
	vec3 direction;
	vec3 Le;
	Light(vec3 _direction, vec3 _Le) {
		direction = normalize(_direction);
		Le = _Le;
	}
};

const float epsilon = 0.0001f;


class Scene {
public:

	void build();
	void render(std::vector<vec4>& image);
	Hit firstIntersect(Ray ray);
	bool shadowIntersect(Ray ray);
	vec3 trace(Ray ray, int depth);
	std::vector<Intersectable *> objects;
	std::vector<Light *> lights;


private:
    
	Camera camera;
	vec3 La;

};
#endif