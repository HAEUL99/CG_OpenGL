#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <array>
#include "ObjModel.h"

ObjModel::ObjModel(const std::string& name, const char * filename, const MaterialGlobal & m)
	: ModelBase{ name, m }, verts{}, faces{},
	bboxMin{}, bboxMax{}
{
	LoadModel(filename);
}

// fills verts and faces arrays, supposes .obj file to have "f " entries without slashes
void ObjModel::LoadModel(const char *filename) {
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) {
		//std::cerr << "Failed to open " << filename << std::endl;
		return;
	}
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v[i];
			verts.push_back(v);
		}
		else if (!line.compare(0, 2, "f ")) {
			Vec3i f;
			int idx, cnt = 0;
			iss >> trash;
			while (iss >> idx) {
				idx--; // in wavefront obj all indices start at 1, not zero
				f[cnt++] = idx;
			}
			if (3 == cnt) faces.push_back(f);
		}
	}
	//std::cerr << "# v# " << verts.size() << " f# " << faces.size() << std::endl;

	get_bbox(bboxMin, bboxMax);
}

bool ObjModel::ray_aabb_intersect(const Vec3f &orig, const Vec3f &dir) const
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
	//std::cerr << t_enter << " " << t_exit << std::endl;
	return t_exit > t_enter && t_exit >= 0;
}

// Moller and Trumbore
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


int ObjModel::nverts() const {
	return (int)verts.size();
}

int ObjModel::nfaces() const {
	return (int)faces.size();
}

void ObjModel::get_bbox(Vec3f &min, Vec3f &max) {
	min = max = verts[0];
	for (int i = 1; i < (int)verts.size(); ++i) {
		for (int j = 0; j < 3; j++) {
			min[j] = std::min(min[j], verts[i][j]);
			max[j] = std::max(max[j], verts[i][j]);
		}
	}
	//std::cerr << "bbox: [" << min << " : " << max << "]" << std::endl;
}

bool ObjModel::RayIntersect(const Vec3f & orig, const Vec3f & dir,
	float & closest, Vec3f& hit, Vec3f& normal) const
{
	if (!ray_aabb_intersect(orig, dir))
		return false;

	float t = std::numeric_limits<float>::max();
	bool isIntersection = false;
	Vec3f tmpNormal;
	for (int fi = 0; fi < nfaces(); fi++)
	{
		float currentT;
		if (ray_triangle_intersect(fi, orig, dir, currentT, tmpNormal))
		{
			if (currentT < t) {
				t = currentT;
				normal = tmpNormal;
				isIntersection = true;

			}
		}

	}

	if (isIntersection) {
		closest = t;
		hit = orig + dir * t;
		return true;
	}

	return false;

}

const Vec3f &ObjModel::point(int i) const {
	assert(i >= 0 && i < nverts());
	return verts[i];
}

Vec3f &ObjModel::point(int i) {
	assert(i >= 0 && i < nverts());
	return verts[i];
}

int ObjModel::vert(int fi, int li) const {
	assert(fi >= 0 && fi < nfaces() && li >= 0 && li < 3);
	//std::cerr << faces[fi][li] << std::endl;
	return faces[fi][li];
}

