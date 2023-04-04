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
                vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                verts.push_back(vertex);
        }
        else if ( strcmp( lineHeader, "f" ) == 0 )
        {
            
            vec3 vertex;
            int matches = fscanf(file, "%d %d %d\n", &vertex.x, &vertex.y, &vertex.z);
            
            faces.push_back(vertex);

        }
    }
	std::cerr << "# v# " << verts.size() << " f# " << faces.size() << std::endl;
	std::cout << "!";
	//SPDLOG_INFO("# vertexes: {},  faces : {}", verts.size(), faces.size());
	get_bbox(bboxMin, bboxMax);
}

void ObjModel::get_bbox(vec3 &min, vec3 &max) {
	min = max = verts[0];
	
	for (int i = 1; i < (int)verts.size(); ++i) {
		vec3 m_verts = verts[i];
		for (int j = 0; j < 3; j++) {
			
			if(j == 0)
			{
				min.x = std::min(min[j], m_verts[j]);
				max.x = std::max(max[j], m_verts[j]);
			}
			if(j==1)
			{
				min.y= std::min(min[j], m_verts[j]);
				max.y = std::max(max[j], m_verts[j]);
			}
			if(j == 2)
			{
				min.z = std::min(min[j], m_verts[j]);
				max.z = std::max(max[j], m_verts[j]);
			}
			
		}
	}
	SPDLOG_INFO("# minx: {},  miny : {}, minz : {}", min.x, min.y, min.z);
	SPDLOG_INFO("# maxx: {},  maxy : {}, maxz : {}", max.x, max.y, max.z);
	//std::cerr << "bbox: [" << min << " : " << max << "]" << std::endl;
}

bool ObjModel::ray_aabb_intersect(const vec3&orig, const vec3 &dir) const
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

bool ObjModel::ray_triangle_intersect(const int &fi, const vec3 &orig, const vec3 &dir, float &tnear, vec3& normal) const {
	vec3 edge1 = point(vert(fi, 1)) - point(vert(fi, 0));
	vec3 edge2 = point(vert(fi, 2)) - point(vert(fi, 0));
	vec3 pvec = cross(dir, edge2);
	float det = edge1.x * pvec.x + edge1.y * pvec.y + edge1.z * pvec.z;
	if (det < 1e-5) return false;

	vec3 tvec = orig - point(vert(fi, 0));
	float u = tvec.x * pvec.x + tvec.y * pvec.y + tvec.z * pvec.z;
	if (u < 0 || u > det) return false;

	vec3 qvec = cross(tvec, edge1);
	float v = dir.x * qvec.x + dir.y * qvec.y + dir.z * qvec.z;
	if (v < 0 || u + v > det) return false;

	tnear = ( edge2.x * qvec.x + edge2.y * qvec.y + edge2.z * qvec.z ) * (1. / det);
	normal = cross(edge1, edge2);
	normal = normal * (1/length(normal));
	return tnear > 1e-5;
}

const vec3 &ObjModel::point(int i) const {
	assert(i >= 0 && i < verts.size());
	return verts[i];
}

vec3 &ObjModel::point(int i) {
	assert(i >= 0 && i < verts.size());
	return verts[i];
}

int ObjModel::vert(int fi, int li) const {
	assert(fi >= 0 && fi < faces.size() && li >= 0 && li < 3);
	vec3 m_faces = faces[fi];
	return m_faces[li];
}



Hit ObjModel::intersect(const Ray& ray)
{
	
    Hit hit;
	
    if (!ray_aabb_intersect(ray.start, ray.dir))
		return hit;
	
    float t = std::numeric_limits<float>::max();
	bool isIntersection = false;
	vec3 tmpNormal;
    for (int fi = 0; fi < faces.size(); fi++)
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
		hit.material = material;
	}
	
	return hit;


}