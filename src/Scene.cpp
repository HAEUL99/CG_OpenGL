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
	objects.push_back(new ObjModel{ std::string{ "Duck" }, "D:/git/CG/opengl_example/model/duck.obj", mirrorMaterial });
	///scene->AddSphere({ std::string{ "Sphere 2" },Vec3f(-1.0f,-1.5f, 4.0f), 2.0f, glassMaterial });
	objects.push_back(new Sphere{ std::string{ "Sphere 2" }, Vec3f(-4.0f, 4.0f, 2.0f), 1.0f, dullGreenMaterial  });
	objects.push_back(new Sphere{ std::string{ "Sphere 3" }, Vec3f(-3.0f, 2.0f, -2.0f), 2.0f, mirrorMaterial  });
	//scene->AddSphere({ std::string{ "Sphere 4" },Vec3f(7.0f, 5.0f, -2.0f), 4.0f, mirrorMaterial });

	//scene->AddObjModel(std::string{ "Duck" }, "D:/raystep/TinyRaytracer_SFML/TinyRatracer_SFML/resources/cow.obj", shinyIvoryMaterial);

	// Add lights in scene
	//scene->AddLight({ Vec3f(-20.0f, 20.0f, 20.0f), 1.5f });
	lights.push_back(new Light{ Vec3f(4.0f, 4.0f, 4.0f), 1.5f });
	mCameraForward = Vec3f(0.0f,0.0f,-1.0f); 
	mOrbitCameraParameter = Vec3f(15.0f, -90.0f * 3.1415917f / 180.0f, 0.0f); 


	mCameraForward.x = cosf(mOrbitCameraParameter.y) * cosf(mOrbitCameraParameter.z);
	mCameraForward.y = sinf(mOrbitCameraParameter.z);
	mCameraForward.z = sinf(mOrbitCameraParameter.y) * cosf(mOrbitCameraParameter.z);
	mCameraForward = mCameraForward.normalize();

	mCameraPosition = -mOrbitCameraParameter.x * mCameraForward;

	mCameraRight = cross(mCameraForward, Vec3f{ 0.0f,1.0f,0.0f }).normalize();
	mCameraUp = cross(mCameraRight, -mCameraForward).normalize();

	//scene->AddLight({ Vec3f(30.0f, 50.0f,-25.0f), 1.8f });
	//scene->AddLight({ Vec3f(30.0f, 20.0f, 30.0f), 1.7f });

	//objects.push_back(new Sphere(vec3(-0.55, 0, 0), 0.5, 
	//						new RoughMaterial(vec3(0.3, 0.2, 0.1), ks, 50)));
	/*
	objects.push_back(new Sphere(vec3(0.55, 0,  0), 0.5, 
						new RoughMaterial(vec3(0.1, 0.2, 0.3), ks, 100)));
	objects.push_back(new Sphere(vec3(0, 0.5, -0.8), 0.5,
						new RoughMaterial(vec3(0.3, 0, 0.2), ks, 20)));
	*/
	//objects.push_back(new Sphere(vec3(0, 0.3,  0.6), 0.5,
	//					new RefractiveMaterial(vec3(1.2, 1.2, 1.2))));
	
				
	//objects.push_back(new Sphere(vec3(0.55, 0,  0), 0.5, 
	//					new ReflectiveMaterial(vec3(0.14, 0.16, 0.13), vec3(2.1, 2.3, 2.1))));

	
	//ObjModel obj = new ObjModel{ std::string{ "cow" }, "D:/git/CG/opengl_example/model/cow.obj", new RoughMaterial(vec3(0.3, 0.2, 0.1), ks, 50) }; 
	//objects.push_back(new ObjModel{ std::string{ "cow" }, "D:/git/CG/opengl_example/model/cube.obj",new RoughMaterial(vec3(0.3, 0.7, 0.4), ks, 50)} );
	
}

void Scene::render(std::vector<Vec4f>& image) {

/*
	unsigned int renderW = 64;
	unsigned int renderH = 48;

	std::vector<vec3> framebuffer(renderH*renderW);
	for (size_t j = 0; j < renderH; j++) {
		for (size_t i = 0; i < renderW; i++) {
			//image[i + j * renderW] = vec3(j / float(renderH), i / float(renderW), 0);
			vec3 color = trace(camera.getRay(renderW, renderH), 0);
			image[i + j * renderW] = vec4(color.x, color.y, color.z, 1);
			framebuffer[i + j * renderW] = vec3(color.x, color.y, color.z);
		}
	}
*/
	
	
	for (int Y = 0; Y < WINDOW_HEIGHT; Y++) 
	{
		#pragma omp parallel for
		for (int X = 0; X < WINDOW_WIDTH; X++) 
		{
			
			//vec3 color = trace(camera.getRay(X, Y), 0);
			//Vec3f color = CastRay(camera.getRay(X, Y).start, camera.getRay(X, Y).dir, 0);
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
		//return mScene->GetEnvironmentColor(direction);
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

	// Add checkerboard
	float checkerboard_dist = std::numeric_limits<float>::max();
	if (fabs(direction.y) > 1e-3) {
		float d = -(origin.y + 4) / direction.y; // the checkerboard plane has equation y = -4
		Vec3f pt = origin + direction * d;
		if (d > 0 && fabs(pt.x) < 10 && pt.z<6 && pt.z>-14 && d < modelDist) {
			checkerboard_dist = d;
			hit = pt;
			normal = Vec3f(0, 1, 0);
			Vec3f diffuseColor = (int(.5*hit.x + 1000) + int(.5*hit.z)) & 1 ? Vec3f(1, 1, 1) : Vec3f(1, .7, .3);
			diffuseColor = diffuseColor * 0.3f;
			material.SetDiffuseColor(diffuseColor);
		}
	}
	return std::min(modelDist, checkerboard_dist) < 1000;
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
