#pragma once

#include "framework.h"
#include "MaterialGlobal.h"
#include "ModelBase.h"


class Sphere : public ModelBase
{
private:
	Vec3f center;
	float radius;

public:
	Sphere(const std::string& name, const Vec3f& c, const float& r, const MaterialGlobal& mat) 
		: ModelBase{ name, mat }, center { c }, radius{ r }
	{

	}

	virtual bool RayIntersect(const Vec3f& orig, const Vec3f& dir, float& closest, Vec3f& hit, Vec3f& normal) const override
	{
		Vec3f L = center - orig; //orig에서 출발하여 center를 향하는 벡터
		float tca = L * dir; // L vector를 dir vector에 projection
		float d2 = L * L - tca * tca; //L^2 = tca^2 + r^2인 경우 접하는 삼각형.
		if (d2 > radius*radius) return false; //r^2보다 거리가 멀면, ray가 원에 부H히지 않음
		float thc = sqrtf(radius*radius - d2);
		float t0 = tca - thc; //t0는 현재 구에 대한 t
		float t1 = tca + thc; //t0와 t1은 가까운/먼 교차점까지의 파라메터
		if (t0 < 0) t0 = t1;  //접점이 하나인 경우 or dir 반대 방향에 원이 있는 경우를 처리
		if (t0 < 0) return false;
		
		if (t0 < closest) //현재 구가 가장 가까운 구인지 확인
		{
			closest = t0;
			hit = orig + dir * closest;
			normal = (hit - center).normalize();
			return true;
		}
		return false;
	}

	
};