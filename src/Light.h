#pragma once

#include "framework.h"



class Light
{
private:
	bool IsLight = true;
	Vec3f position;
	float intensity;
	Vec3f ambient = { 0.0f, 0.0f, 0.0f };
	Vec3f specular = { 0.0f, 0.0f, 0.0f };



public:
	//point light
	Light(const Vec3f& p, const float& i)
		: position{ p }, intensity{ i }
	{

	}

	inline bool GetIsLight() const { return IsLight; }
	inline Vec3f GetPosition() const { return position; }
	inline float GetIntensity() const { return intensity; }
	inline Vec3f GetAmbeint() const { return ambient; }
	inline Vec3f GetSpecular() const { return specular; }


};