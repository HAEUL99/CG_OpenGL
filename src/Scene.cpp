#include "framework.h"
#include "Scene.h"
#include "Sphere.h"
#include "Camera.h"
#include <iostream>
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
	vec3 eye = vec3(0, 0, 4), vup = vec3(0, 1, 0), lookat = vec3(0, 0, 0);
	float fov = 45 * 3.14159265 / 180.0f;
	camera.set(eye, lookat, vup, fov);

	La = vec3(0.4f, 0.4f, 0.4f);
	vec3 lightDirection(1, 1, 1), Le(2, 2, 2);
	lights.push_back(new Light(lightDirection, Le));

	vec3 ks(2, 2, 2);
	
	//bjects.push_back(new Sphere(vec3(-0.55, 0, 0), 0.5, 
	//						new RoughMaterial(vec3(0.3, 0.2, 0.1), ks, 50)));
	/*
	objects.push_back(new Sphere(vec3(0.55, 0,  0), 0.5, 
						new RoughMaterial(vec3(0.1, 0.2, 0.3), ks, 100)));
	objects.push_back(new Sphere(vec3(0, 0.5, -0.8), 0.5,
						new RoughMaterial(vec3(0.3, 0, 0.2), ks, 20)));
	
	objects.push_back(new Sphere(vec3(0, 0.3,  0.6), 0.5,
						new RefractiveMaterial(vec3(1.2, 1.2, 1.2))));
	*/
				
	//objects.push_back(new Sphere(vec3(0.55, 0,  0), 0.5, 
	//					new ReflectiveMaterial(vec3(0.14, 0.16, 0.13), vec3(4.1, 2.3, 3.1))));

	
	//ObjModel obj = new ObjModel{ std::string{ "cow" }, "D:/git/CG/opengl_example/model/cow.obj", new RoughMaterial(vec3(0.3, 0.2, 0.1), ks, 50) }; 
	objects.push_back(new ObjModel{ std::string{ "cow" }, "D:/git/CG/opengl_example/model/cow.obj",new RoughMaterial(vec3(0.3, 0.2, 0.1), ks, 50)} );
}

void Scene::render(std::vector<vec4>& image) {
	for (int Y = 0; Y < WINDOW_HEIGHT/2; Y++) 
	{
		#pragma omp parallel for
		for (int X = 0; X < WINDOW_WIDTH/2; X++) 
		{
			vec3 color = CastRay(mCameraPosition, dir, isPreview, 0);
			vec3 color = trace(camera.getRay(X, Y), 0);
			//vec3 color = (0.2f, 0.2, 0.4f);
			image[Y * WINDOW_WIDTH + X] = vec4(color.x, color.y, color.z, 1);
			
		
		}
	}

}

Hit Scene::firstIntersect(Ray ray) 
{
	Hit bestHit;
	for (Intersectable * object : objects) {
		Hit hit = object->intersect(ray); //  hit.t < 0 if no intersection
		if (hit.t > 0 && (bestHit.t < 0 || hit.t < bestHit.t))  bestHit = hit;
	}
	if (dot(ray.dir, bestHit.normal) > 0) bestHit.normal = bestHit.normal * (-1);
	return bestHit;
}

bool Scene::shadowIntersect(Ray ray) {	// for directional lights
	for (Intersectable * object : objects) if (object->intersect(ray).t > 0) return true;
	return false;
}

vec3 Scene::trace(Ray ray, int depth) {
	if (depth > 2) return La;
	Hit hit = firstIntersect(ray);
	if (hit.t < 0) return La;

	if (hit.material->type == ROUGH) {
		vec3 outRadiance = hit.material->ka * La;
		for (Light * light : lights) {
			Ray shadowRay(hit.position + hit.normal * epsilon, light->direction);
			float cosTheta = dot(hit.normal, light->direction);
			if (cosTheta > 0 && !shadowIntersect(shadowRay)) {	// shadow computation
				outRadiance = outRadiance + light->Le * hit.material->kd * cosTheta;
				vec3 halfway = normalize(-ray.dir + light->direction);
				float cosDelta = dot(hit.normal, halfway);
				if (cosDelta > 0) outRadiance = outRadiance + light->Le * hit.material->ks * powf(cosDelta, hit.material->shininess);
			}
		}
		return outRadiance;
	}

	float cosa = -dot(ray.dir, hit.normal);
	vec3 one(1, 1, 1);
	vec3 F = hit.material->F0 + (one - hit.material->F0) * pow(1 - cosa, 5);
	vec3 reflectedDir = ray.dir - hit.normal * dot(hit.normal, ray.dir) * 2.0f;
	vec3 outRadiance = trace(Ray(hit.position + hit.normal * epsilon, reflectedDir), depth + 1) * F;

	if (hit.material->type == REFRACTIVE) {
		float disc = 1 - (1 - cosa * cosa) / hit.material->ior / hit.material->ior; // scalar n
		if (disc >= 0) {
			vec3 refractedDir = ray.dir / hit.material->ior + hit.normal * (cosa / hit.material->ior - sqrt(disc));
			outRadiance = outRadiance +
				trace(Ray(hit.position - hit.normal * epsilon, refractedDir), depth + 1) * (one - F);
		}
	}
	return outRadiance;
}

vec3 Scene::CastRay(const vec3 & origin, const vec3 & direction, size_t currentDepth) const
{
	vec3 hit, normal;
	MaterialGlobal material;

	
	if (currentDepth > maxDepth || !SceneIntersect(origin, direction, hit, normal, material))
	{
		vec3{ 0.2f, 0.7f, 0.8f };

	}
	

	//---Reflection
	vec3 reflectDir = Reflect(direction, normal).normalize();
	vec3 reflectOrigin = dot(reflectDir, normal) < 0 ? hit - normal * 1e-3 : hit + normal * 1e-3;
	vec3 reflectColor = CastRay(reflectOrigin, reflectDir, isPreview, currentDepth + 1);

	

	float diffuseIntensity = 0;
	float specularIntensity = 0;

	for (const Light* const light : mScene->GetLights())
	{
		Vec3f lightDir = (light->GetPosition() - hit).normalize();
		float lightDist = (light->GetPosition() - hit).norm();

		//Shadow evaluation
		Vec3f shadowOrigin = (lightDir * normal < 0) ? hit - normal * 1e-3 : hit + normal * 1e-3;
		Vec3f shadowLightDir = (light->GetPosition() - shadowOrigin).normalize();

		// 지금 그리려는 픽셀에서 빛 방향으로 다시 ray 발사
		Vec3f shadowHit, shadowNormal;
		Material tempMat;
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

	//Utility::SaturateColor(color);
	color.x = std::min(color.x, 1.0f);
	color.y = std::min(color.y, 1.0f);
	color.z = std::min(color.z, 1.0f);

	return color;
}

bool Scene::SceneIntersect(const vec3 & origin, const vec3 direction, 
	vec3 & hit, vec3 & normal, MaterialGlobal & material) const
{
	float modelDist = std::numeric_limits<float>::max();

	vec3 fillColor{};
	bool filled = false;
	vec3 checkHit;
	vec3 checkNormal;

	//for (const ModelBase* const m : mScene->GetObjects())
	for(Intersectable * object : objects)
	{
		if (object->intersect(origin, direction, modelDist, checkHit, checkNormal))
		{
			hit = checkHit;
			normal = checkNormal;
			material = object->material;
		}
	}

	
	return modelDist < 1000;
}

vec3 Scene::Reflect(const vec3 & l, const vec3 & n) const
{
	return l - n * 2.0f*(n*l);
}

vec3 Scene::Refract(const vec3& I, const vec3& N, const float etat, float etai) const
{
	float cosi = -std::max(-1.0f, std::min(1.0f, dot(I, N)));
	
	if (cosi < 0.0f)
	{
		return Refract(I, -N, etai, etat);
	}
	float eta = etai / etat;
	float k = 1.0f - eta * eta*(1.0f - cosi * cosi);

	return k < 0 ? vec3{ 1, 0, 0 } : I * eta + N * (eta*cosi - sqrtf(k));
}