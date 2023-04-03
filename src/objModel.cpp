#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <array>
#include "ObjModel.h"



ObjModel::ObjModel(std::string& _name, char * _filename, MaterialGlobal* _m)
{
    name = _name;
    material =_m;

	LoadModel(_filename);
}

// fills verts and faces arrays, supposes .obj file to have "f " entries without slashes
void ObjModel::LoadModel(char *filename) 
{
	FILE* file = fopen(filename, "r");


    while( 1 )
    {

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.
        
        if ( strcmp( lineHeader, "v" ) == 0 )
        {
                Vec3f vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                verts.push_back(vertex);
        }
        else if ( strcmp( lineHeader, "f" ) == 0 )
        {
            
            Vec3i vertex;
            int matches = fscanf(file, "%d %d %d\n", &vertex.x, &vertex.y, &vertex.z);
            
            faces.push_back(vertex);

        }
    }
	//std::cerr << "# v# " << verts.size() << " f# " << faces.size() << std::endl;

	get_bbox(bboxMin, bboxMax);
}

void ObjModel::get_bbox(Vec3f &min, Vec3f &max) {
	min = max = verts[0];
	for (int i = 1; i < (int)verts.size(); ++i) {
		for (int j = 0; j < 3; j++) {
			min[j] = std::min(min[j], verts[i][j]);
			max[j] = std::max(max[j], verts[i][j]);
		}
	}
    //SPDLOG_INFO("# vertexes: {},  vertexes1 : {}", min.x, max.x);
	//std::cerr << "bbox: [" << min << " : " << max << "]" << std::endl;
}

bool ObjModel::ray_aabb_intersect(const Vec3f&orig, const Vec3f &dir) const
{
	float t_min_x = (bboxMin.x - orig.x) / dir.x;
	float t_min_y = (bboxMin.y - orig.y) / dir.y;
	float t_min_z = (bboxMin.z - orig.z) / dir.z;

	float t_max_x = (bboxMax.x - orig.x) / dir.x;
	float t_max_y = (bboxMax.y - orig.y) / dir.y;
	float t_max_z = (bboxMax.z - orig.z) / dir.z;

	if (dir.x < 0) {
		std::swap(t_max_x, t_min_x);
	}
	if (dir.y < 0) {
		std::swap(t_max_y, t_min_y);
	}
	if (dir.z < 0) {
		std::swap(t_max_z, t_min_z);
	}

	float t_enter = std::max(t_min_x, std::max(t_min_y, t_min_z));
	float t_exit = std::min(t_max_x, std::min(t_max_y, t_max_z));
	return t_exit > t_enter && t_exit >= 0;
}

bool ObjModel::ray_triangle_intersect(const int &fi, const Vec3f &orig, const Vec3f &dir, float &tnear, Vec3f& normal) const {
	Vec3f edge1 = point(vert(fi, 1)) - point(vert(fi, 0));
	Vec3f edge2 = point(vert(fi, 2)) - point(vert(fi, 0));
	Vec3f pvec = cross(dir, edge2);
	float det = edge1 * pvec;
	if (det < 1e-5) return false;

	Vec3f tvec = orig - point(vert(fi, 0));
	float u = tvec * pvec;
	if (u < 0 || u > det) return false;

	Vec3f qvec = cross(tvec, edge1);
	float v = dir * qvec;
	if (v < 0 || u + v > det) return false;

	tnear = edge2 * qvec * (1. / det);
	normal = cross(edge1, edge2).normalize();
	return tnear > 1e-5;
}

Hit ObjModel::intersect(const Ray& ray)
{
	
    Hit hit;
	/*
    if (!ray_aabb_intersect(ray.start, ray.dir)
		return hit;
	
    float t = std::numeric_limits<float>::max();
	bool isIntersection = false;
	glm::vec3 tmpNormal;
    for (int fi = 0; fi < nfaces(); fi++)
	{
		float currentT;
		if (ray_triangle_intersect(fi, ray.start, ray.dir, currentT, tmpNormal))
		{
			if (currentT < t) {
				t = currentT;
				hit.normal = tmpNormal;
				isIntersection = true;

			}
		}

	}

    if (isIntersection) {
		hit.t = t;
		hit.position = ray.start + ray.dir * t;
	
	}
	*/
	return hit;


}

const Vec3f &ObjModel::point(int i) const {
	assert(i >= 0 && i < verts.size());
	return verts[i];
}

Vec3f &ObjModel::point(int i) {
	assert(i >= 0 && i < verts.size());
	return verts[i];
}

int ObjModel::vert(int fi, int li) const {
	assert(fi >= 0 && fi < faces.size() && li >= 0 && li < 3);
	return faces[fi][li];
}
