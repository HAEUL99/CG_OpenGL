#pragma once

#include "framework.h"


class MaterialGlobal
{
private:
	Vec4f albedo;
	Vec3f diffuseColor;
	float specularExponent;
	float refractiveIndex;

public:
	MaterialGlobal(const Vec4f& a, const Vec3f& color, const float spec, const float r)
		: albedo{ a }, diffuseColor{ color }, specularExponent{ spec }, refractiveIndex{ r }
	{

	}

	MaterialGlobal()
		: albedo{ 1.0f, 0.0f, 0.0f, 0.0f }, diffuseColor{ 1.0f, 1.0f, 0.0f }, 
		specularExponent{ 1.0f }, refractiveIndex{ 1.0f }
	{

	}

	inline Vec4f GetAlbedo() const { return albedo; }
	inline Vec3f GetDiffuseColor() const { return diffuseColor; }
	inline void SetDiffuseColor(const Vec3f color) { diffuseColor = color; } // for checkerboard
	inline float GetSpecularExponent() const { return specularExponent; }
	inline float GetRefractiveIndex() const { return refractiveIndex; }

};