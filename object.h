#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "utils.h"
#include "ray.h"
#include "intersectData.h"
#include "illumination.h"

enum OBJECT_TYPE 
{
	SPHERE,
	POLYGON,
};



// Object abstract class
struct Object 
{
	IlluminationModel 	*i_model;

	u32 type;	

	/**
	 * Virtual destructor.
	 */
	virtual ~Object(void) {};


	/**
	 * Checks for the intersection of an object with the given ray, if there is
	 * an intersection then the params intersection and normal will have their
	 * contents modified.
	 *
	 * @param r: The ray that will be checked for intersection.
	 * @param intersection: The reference to the vector that will hold the 
	 * intersection point if there is an intersection.
	 * @param normal: The reference to the vector that will hold the surface
	 * normal if there is an intersection.
	 * @return: True if the object was intersected, false otherwise.
	 */
	virtual bool intersect(const Ray &r, IntersectData &id) = 0;
	
};

class Sphere : public Object
{
	f32 radius;
public:
	glm::vec4 center;
	
	Sphere(glm::vec4 c, f32 r, IlluminationModel *i);
	~Sphere(void);

	/**
	 * Checks for the intersection of an object with the given ray, if there is
	 * an intersection then the params intersection and normal will have their
	 * contents modified.
	 *
	 * @param r: The ray that will be checked for intersection.
	 * @param intersection: The reference to the vector that will hold the 
	 * intersection point if there is an intersection.
	 * @param normal: The reference to the vector that will hold the surface
	 * normal if there is an intersection.
	 * @return: True if the object was intersected, false otherwise.
	 */
	bool intersect(const Ray &r, IntersectData &id);
};

class Polygon : public Object
{
    // triangles will be stored as groups of 3 vec3s
public:
    std::vector<glm::vec4> vertices;

	/**
	 * Constructor.
	 *
	 * @param v: The list of verticies, triangles will be in groups of 3.
	 * @param n: The normal for the triangle face.
	 */
	Polygon(std::vector<glm::vec4> v, IlluminationModel *i);

	/**
	 * Destructor.
	 */
	~Polygon(void);

	/**
	 * Checks for the intersection of an object with the given ray, if there is
	 * an intersection then the params intersection and normal will have their
	 * contents modified.
	 *
	 * @param r: The ray that will be checked for intersection.
	 * @param intersection: The reference to the vector that will hold the 
	 * intersection point if there is an intersection.
	 * @param normal: The reference to the vector that will hold the surface
	 * normal if there is an intersection.
	 * @return: True if the object was intersected, false otherwise.
	 */
	bool intersect(const Ray &r, IntersectData &id);
};
#endif
