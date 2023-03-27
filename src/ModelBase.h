#pragma once

#include "Material.h"

class ModelBase
{
protected:
	std::string name;
	Material material;

public:
	ModelBase(const std::string& name, const Material& m) : name{ name }, material { m } {}
	virtual bool RayIntersect(const Vec3f& orig, const Vec3f& dir,
		float& closest, Vec3f& hit, Vec3f& normal) const = 0;

	virtual bool EditModel() = 0;
public:
	inline const Material& GetMaterial() const { return material; }
	inline const std::string& GetName() const { return name; }
};