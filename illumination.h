#ifndef ILLUMINATION_H
#define ILLUMINATION_H

#include <glm/glm.hpp>
#include "intersectData.h"

struct Light
{
	glm::vec4 	position;
	glm::vec4	color;
	glm::vec4	ambient;

	Light(glm::vec4 p, glm::vec4 c, glm::vec4 a);
	~Light(void);
};

class IlluminationModel {
protected:
	f32	ambient_coef;
	f32	specular_coef;
	f32	diffuse_coef;

	f32 specular_exp;
	
public:
	f32 reflection_const;
	f32 refraction_const;
	u32 type;

	IlluminationModel(f32 a_coef, f32 s_coef, f32 d_coef, f32 s_exp, 
			f32 r_const, f32 t_const);
	virtual ~IlluminationModel(void) {};

	virtual glm::vec4 illuminate(IntersectData &id, Light light, 
			glm::vec3 eyepoint, bool in_shadow) = 0;
};

class Phong : public IlluminationModel {
	glm::vec4 ambient_mat;
	glm::vec4 diffuse_mat;
	glm::vec4 specular_mat;
public:	
	Phong(void);
	Phong(f32 a_coef, f32 s_coef, f32 d_coef, f32 s_exp, f32 r_const, 
			f32 t_const, glm::vec4 aM, glm::vec4 dM, glm::vec4 sM);
	~Phong(void);

	glm::vec4 illuminate(IntersectData &id, Light light, glm::vec3 eyepoint, 
			bool in_shadow);
};

class CheckerBoard : public IlluminationModel {
	glm::vec4 color1;
	glm::vec4 color2;
public:

	CheckerBoard(f32 a_coef, f32 s_coef, f32 d_coef, f32 s_exp, 
			f32 r_const, f32 t_const, glm::vec4 &c1, glm::vec4 &c2);
	~CheckerBoard(void);

	glm::vec4 illuminate(IntersectData &id, Light light, glm::vec3 eyepoint,
			bool in_shadow);

private:
	glm::vec4 get_cube(IntersectData &id);
};

#endif
