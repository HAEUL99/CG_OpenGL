#include "framework.h"
#include "Scene.h"
#include "Sphere.h"
#include "Camera.h"
#include <iostream>
#include <fstream>
#include "objModel.h"


/*
SceneUPtr Scene::Create()
{
	auto scene = SceneUPtr(new Scene());
	return std::move(scene);

}
*/
void Scene::build()
{
	Vec3f eye = Vec3f(0, 0, 7), vup = Vec3f(0, 1, 0), lookat = Vec3f(0, 0, 0);
	fov = 45 * 3.14159265 / 180.0f;
	camera.set(eye, lookat, vup, fov);

	const MaterialGlobal shinyIvoryMaterial{ Vec4f{0.6f, 0.3f, 0.1f, 0.0f }, Vec3f{0.4f,0.4f,0.3f}, 50.0f, 1.0f };
	const MaterialGlobal dullGreenMaterial{ Vec4f{0.7f, 0.1f, 0.0f, 0.0f }, Vec3f{0.2f,0.5f,0.1f}, 5.0f, 1.0f };
	const MaterialGlobal mirrorMaterial{ Vec4f{0.0f, 10.0f, 0.8f, 0.0f }, Vec3f{1.0f,1.0f,1.0f}, 1425.0f, 1.0f };
	const MaterialGlobal glassMaterial{ Vec4f{0.0f, 0.5f, 0.1f, 0.8f }, Vec3f{0.6f,0.7f,0.8f}, 125.0f, 1.5f };


		// Add objects in scene
	//scene->AddSphere({ std::string{ "Sphere 1" }, Vec3f(-3.0f, 0.0f, 0.0f), 2.0f, shinyIvoryMaterial });
	
	objects.push_back(new Sphere{ std::string{ "Sphere 1" }, Vec3f(0.0f, 0.0f, 0.0f), 2.0f, shinyIvoryMaterial  });
	
	//objects.push_back(new ObjModel{ std::string{ "Duck" }, "D:/git/CG/opengl_example/model/duck.obj", mirrorMaterial });
	
	objects.push_back(new Sphere{ std::string{ "Sphere 2" }, Vec3f(-4.0f, 4.0f, 2.0f), 1.0f, dullGreenMaterial  });
	objects.push_back(new Sphere{ std::string{ "Sphere 3" }, Vec3f(-3.0f, 2.0f, -2.0f), 2.0f, mirrorMaterial  });
	//scene->AddSphere({ std::string{ "Sphere 4" },Vec3f(7.0f, 5.0f, -2.0f), 4.0f, mirrorMaterial });

	//scene->AddObjModel(std::string{ "Duck" }, "D:/raystep/TinyRaytracer_SFML/TinyRatracer_SFML/resources/cow.obj", shinyIvoryMaterial);

	// Add lights in scene
	//scene->AddLight({ Vec3f(-20.0f, 20.0f, 20.0f), 1.5f });
	lights.push_back(new Light{ Vec3f(0.0f, 0.0f, 10.0f), 1.5f });
	mCameraForward = Vec3f(0.0f,0.0f,-1.0f); 
	mOrbitCameraParameter = Vec3f(15.0f, -90.0f * 3.1415917f / 180.0f, 0.0f); 


	mCameraForward.x = cosf(mOrbitCameraParameter.y) * cosf(mOrbitCameraParameter.z);
	mCameraForward.y = sinf(mOrbitCameraParameter.z);
	mCameraForward.z = sinf(mOrbitCameraParameter.y) * cosf(mOrbitCameraParameter.z);
	mCameraForward = mCameraForward.normalize();

	mCameraPosition = -mOrbitCameraParameter.x * mCameraForward + Vec3f(0.0f, 1.0f, 1.0f);

	mCameraRight = cross(mCameraForward, Vec3f{ 0.0f,1.0f,0.0f }).normalize();
	mCameraUp = cross(mCameraRight, -mCameraForward).normalize();

	
	
}

void Scene::render(std::vector<Vec4f>& image) {

	for (int Y = 0; Y < WINDOW_HEIGHT; Y++) 
	{
		#pragma omp parallel for
		for (int X = 0; X < WINDOW_WIDTH; X++) 
		{
			
			float x = (2 * (X + 0.5f) / (float)WINDOW_WIDTH - 1) * tan(fov / 2.0f)*WINDOW_WIDTH / (float)WINDOW_HEIGHT;
			float y = -(2 * (Y + 0.5f) / (float)WINDOW_HEIGHT - 1) * tan(fov / 2.0f);
			Vec3f dir = ((mCameraRight * x) + (mCameraUp * y) + mCameraForward).normalize();
			
			Vec3f color = CastRay(mCameraPosition, dir, 0);
			image[Y * WINDOW_WIDTH + X].x = color.x; //Depth 0부터 시작
			image[Y * WINDOW_WIDTH + X].y = color.y; //Depth 0부터 시작
			image[Y * WINDOW_WIDTH + X].z = color.z; //Depth 0부터 시작
			image[Y * WINDOW_WIDTH + X].w = 1.0f; //Depth 0부터 시작
		}
	}
	
	

}

Vec3f Scene::CastRay(const Vec3f & origin, const Vec3f & direction, size_t currentDepth) const
{
	Vec3f hit, normal;
	MaterialGlobal material;


	if (currentDepth > maxDepth || !SceneIntersect(origin, direction, hit, normal, material))
	{
		return Vec3f(0.2f, 0.2f, 0.2f);
	}
	

	//---Reflection
	Vec3f reflectDir = Reflect(direction, normal).normalize();
	Vec3f reflectOrigin = reflectDir * normal < 0 ? hit - normal * 1e-3 : hit + normal * 1e-3;
	Vec3f reflectColor = CastRay(reflectOrigin, reflectDir, currentDepth + 1);

	//---Refraction
	Vec3f refractDir = Refract(direction, normal, material.GetRefractiveIndex()).normalize();
	Vec3f refractOrigin = refractDir * normal < 0 ? hit - normal * 1e-3 : hit + normal * 1e-3;
	Vec3f refractColor = CastRay(refractOrigin, refractDir, currentDepth + 1);

	float diffuseIntensity = 0;
	float specularIntensity = 0;

	for (const Light* const light : lights)
	{
		Vec3f lightDir = (light->GetPosition() - hit).normalize();
		float lightDist = (light->GetPosition() - hit).norm();

		//Shadow evaluation
		Vec3f shadowOrigin = (lightDir * normal < 0) ? hit - normal * 1e-3 : hit + normal * 1e-3;
		Vec3f shadowLightDir = (light->GetPosition() - shadowOrigin).normalize();

		// 지금 그리려는 픽셀에서 빛 방향으로 다시 ray 발사
		Vec3f shadowHit, shadowNormal;
		MaterialGlobal tempMat;
		if (SceneIntersect(shadowOrigin, shadowLightDir, shadowHit, shadowNormal, tempMat)
			&& lightDist > (shadowHit - shadowOrigin).norm()) //다른 물체와 먼저 교차한 경우
		{
			continue;
		}

		diffuseIntensity += light->GetIntensity() * std::max(0.0f, lightDir*normal);
		specularIntensity += std::powf(std::max(0.0f, Reflect(lightDir, normal)*direction), material.GetSpecularExponent())
							* light->GetIntensity();
	}

	Vec4f materialAlbedo = material.GetAlbedo();
	Vec3f color = material.GetDiffuseColor() * diffuseIntensity * materialAlbedo[0] // diffuse term
		+ Vec3f{ 1.0f, 1.0f, 1.0f }*specularIntensity * materialAlbedo[1] // specular term
		+ reflectColor * materialAlbedo[2] // reflect term
		+ refractColor * materialAlbedo[3]; // refract term
	
	color.x = std::min(color.x, 1.0f);
	color.y = std::min(color.y, 1.0f);
	color.z = std::min(color.z, 1.0f);

	return color;
}

bool Scene::SceneIntersect(const Vec3f & origin, const Vec3f direction, 
	Vec3f & hit, Vec3f & normal, MaterialGlobal & material) const
{
	float modelDist = std::numeric_limits<float>::max();

	Vec3f fillColor{};
	bool filled = false;
	Vec3f checkHit;
	Vec3f checkNormal;

	for (const ModelBase* const m : objects)
	{
		if (m->RayIntersect(origin, direction, modelDist, checkHit, checkNormal))
		{
			hit = checkHit;
			normal = checkNormal;
			material = m->GetMaterial();
		}
	}

	float checkerboard_dists[4];
	// Add checkerboard
	checkerboard_dists[0] = std::numeric_limits<float>::max();
	if (fabs(direction.y) > 1e-3) {
		float d = -(origin.y + 8) / direction.y; // the checkerboard plane has equation y = -4
		Vec3f pt = origin + direction * d;
		if (d > 0 && fabs(pt.x) < 10 && pt.z<6 && pt.z>-14 && d < modelDist) {
		//if(d>0 && d< modelDist && std::abs(pt.x)<10 && pt.z<0 && pt.z>-14){
			checkerboard_dists[0] = d;
			hit = pt;
			normal = Vec3f(0, 1, 0);
			Vec3f diffuseColor;
			diffuseColor = (int(.5*hit.x + 1000) + int(.5*hit.z)) & 1 ? Vec3f(1, 1, 1) : Vec3f(0.1f, 0.6f, 0.3f);
			
			
			diffuseColor = diffuseColor * 0.3f;
			material.SetDiffuseColor(diffuseColor);
		}
	}

	
	


	checkerboard_dists[1] = std::numeric_limits<float>::max();
	if (fabs(direction.x) > 1e-3) {
		float d = -(origin.x + 10) / direction.x; // the checkerboard plane has equation x = -10
		Vec3f pt = origin + direction * d;
		if (d > 0 && fabs(pt.y) < 8 && pt.z<6 && pt.z>-14 && d < modelDist) {
			checkerboard_dists[1] = d;
			hit = pt;
			normal = Vec3f(1, 0, 0);
			Vec3f diffuseColor = (int(.5*hit.y + 1000) + int(.5*hit.z)) & 1 ? Vec3f(1, 1, 1) : Vec3f(0.1f, 0.6f, 0.3f);
			diffuseColor = diffuseColor * 0.3f;
			material.SetDiffuseColor(diffuseColor);
		}
	}

	
	checkerboard_dists[2] = std::numeric_limits<float>::max();
	if (fabs(direction.x) > 1e-3) {
		float d = -(origin.x - 10) / direction.x; // the checkerboard plane has equation x = 10
		Vec3f pt = origin + direction * d;
		if (d > 0 && fabs(pt.y) < 8 && pt.z<6 && pt.z>-14 && d < modelDist) {
			checkerboard_dists[2] = d;
			hit = pt;
			normal = Vec3f(-1, 0, 0);
			Vec3f diffuseColor = (int(.5*hit.y + 1000) + int(.5*hit.z)) & 1 ? Vec3f(1, 1, 1) : Vec3f(0.1f, 0.6f, 0.3f);
			diffuseColor = diffuseColor * 0.3f;
			material.SetDiffuseColor(diffuseColor);
		}
	}

	checkerboard_dists[3] = std::numeric_limits<float>::max();
	if (fabs(direction.z) > 1e-3) {
		float d = -(origin.z + 14) / direction.z; // the checkerboard plane has equation z = -14
		Vec3f pt = origin + direction * d;
		if (d > 0 && fabs(pt.y) < 8 && pt.x<10 && pt.x>-10 && d < modelDist) {
			checkerboard_dists[3] = d;
			hit = pt;
			normal = Vec3f(0, 0, 1);
			Vec3f diffuseColor = (int(.5*hit.y + 1000) + int(.5*hit.x)) & 1 ? Vec3f(1, 1, 1) : Vec3f(0.1f, 0.6f, 0.3f);
			diffuseColor = diffuseColor * 0.3f;
			material.SetDiffuseColor(diffuseColor);
		}
	}


	// checkerboard_dist1, checkerboard_dist2
	float minVal = modelDist;
	for(int i = 0; i < 4; i++)
	{
		minVal = std::min(minVal, checkerboard_dists[i]);

	}
	return minVal < 1000;
}

Vec3f Scene::Reflect(const Vec3f & l, const Vec3f & n) const
{
	return l - n * 2.0f*(n*l);
}

Vec3f Scene::Refract(const Vec3f& I, const Vec3f& N, const float etat, float etai) const
{
	float cosi = -std::max(-1.0f, std::min(1.0f, I*N));
	
	if (cosi < 0.0f)
	{
		return Refract(I, -N, etai, etat);
	}
	float eta = etai / etat;
	float k = 1.0f - eta * eta*(1.0f - cosi * cosi);

	return k < 0 ? Vec3f{ 1, 0, 0 } : I * eta + N * (eta*cosi - sqrtf(k));
}
