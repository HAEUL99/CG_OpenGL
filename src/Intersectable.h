#ifndef MARKER1
#define MARKER1
#include "framework.h"

enum MaterialType { ROUGH, REFLECTIVE, REFRACTIVE };

struct MaterialGlobal {
	vec3 ka, kd, ks;
	float  shininess;
	vec3 F0;
	float ior;
	MaterialType type;
	MaterialGlobal(MaterialType t) { type = t; }
};

struct RoughMaterial : MaterialGlobal {
	RoughMaterial(vec3 _kd, vec3 _ks, float _shininess) : MaterialGlobal(ROUGH) {
		ka = _kd *  1;//M_PI; 
		kd = _kd;
		ks = _ks;
		shininess = _shininess;
	}
};

/*
vec3 operator/(vec3 num, vec3 denom) {
	return vec3(num.x / denom.x, num.y / denom.y, num.z / denom.z);
}
*/

struct ReflectiveMaterial : MaterialGlobal {
	ReflectiveMaterial(vec3 n, vec3 kappa) : MaterialGlobal(REFLECTIVE) {
		vec3 one(1, 1, 1);
        F0 = vec3((n - one)*(n - one) + kappa * kappa).x/ ((n + one)*(n + one) + kappa * kappa).x, ((n - one)*(n - one) + kappa * kappa).y/((n + one)*(n + one) + kappa * kappa).y, ((n - one)*(n - one) + kappa * kappa).z/ (((n + one)*(n + one) + kappa * kappa).z);
		//F0 = ((n - one)*(n - one) + kappa * kappa) / ((n + one)*(n + one) + kappa * kappa);
	}
};

struct RefractiveMaterial : MaterialGlobal {
	RefractiveMaterial(vec3 n) : MaterialGlobal(REFRACTIVE) {
		vec3 one(1, 1, 1);
		//F0 = ((n - one)*(n - one)) / ((n + one)*(n + one));
        F0 = vec3(((n - one)*(n - one)).x/ ((n + one)*(n + one)).x, ((n - one)*(n - one)).y/ ((n + one)*(n + one)).y, ((n - one)*(n - one)).z)/ ((n + one)*(n + one)).z;
		ior = n.x;
	}
};

struct Hit {
	float t;
	vec3 position, normal;
	MaterialGlobal * material;
	Hit() { t = -1; }
};

struct Ray {
	vec3 start, dir;
	Ray(vec3 _start, vec3 _dir) { start = _start; dir = normalize(_dir); }
};

class Intersectable {
protected:
	MaterialGlobal* material;
public:
	virtual Hit intersect(const Ray& ray) = 0;
};


#endif
