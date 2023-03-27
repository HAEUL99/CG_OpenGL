#pragma once

#include "Geometry.h"

#include "../imgui-sfml/imgui.h"
#include "../imgui-sfml/imgui-SFML.h"

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

	bool EditMaterial()
	{
		bool e1 = ImGui::DragFloat3("albedo", &this->albedo.x, 0.01f, 0.0f, 1.0f);
		bool e2 = ImGui::ColorEdit3("color(diffuse)", &this->diffuseColor.x);
		bool e3 = ImGui::SliderFloat("specular exponent", &this->specularExponent, 1.0f, 1000.0f);
		bool e4 = ImGui::SliderFloat("refractive index", &this->refractiveIndex, 1.0f, 2.0f);
		return e1 | e2 | e3 | e4;
	}
};