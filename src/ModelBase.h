#pragma once

#include "MaterialGlobal.h"

class ModelBase
{
protected:
	std::string name;
	MaterialGlobal material;

public:
	ModelBase(const std::string& name, const MaterialGlobal& m) : name{ name }, material { m } {}
	virtual bool RayIntersect(const Vec3f& orig, const Vec3f& dir,
		float& closest, Vec3f& hit, Vec3f& normal) const = 0;

public:
	inline const MaterialGlobal& GetMaterial() const { return material; }
	inline const std::string& GetName() const { return name; }
};