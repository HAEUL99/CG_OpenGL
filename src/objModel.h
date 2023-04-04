#pragma once
// From https://github.com/ssloy/tinyraytracer/blob/homework_assignment/model.h
#include <vector>
#include <string>
#include "Intersectable.h"
//#include "Geometry.h"
#include "framework.h"
class ObjModel : public Intersectable {
private:
	
	vec3 bboxMin;
	vec3 bboxMax;
    std::string name;
public:
	std::vector<vec3> verts;
	std::vector<glm::ivec3> faces;
	ObjModel(std::string& name, char* filename, MaterialGlobal* m);
	void LoadModel(char *filename);

	int nverts() const;                          // number of vertices
	int nfaces() const;                          // number of triangles

	bool ray_triangle_intersect(const int &fi, const vec3 &orig, const vec3 &dir, float &tnear, vec3& normal) const;
	bool ray_aabb_intersect(const vec3 &orig, const vec3 &dir) const;

	const vec3 &point(int i) const;                   // coordinates of the vertex i
	vec3 &point(int i);                   // coordinates of the vertex i
    int vert(int fi, int li) const;              // index of the vertex for the triangle fi and local index li
	void get_bbox(vec3 &min, vec3 &max); // bounding box for all the vertices, including isolated ones

public:
	//virtual bool RayIntersect(const vec3& orig, const vec3& dir, 
    //    float& closest, vec3& hit, vec3& normal) const override;
    Hit intersect(const Ray& ray);

};
