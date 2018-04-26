#include "illumination.h"

Light::Light(glm::vec4 p, glm::vec4 c, glm::vec4 a)
	: position(p), color(c), ambient(a)
{

}
Light::~Light(void)
{

}

IlluminationModel::IlluminationModel(f32 a_coef, f32 s_coef, f32 d_coef, 
		f32 s_exp, f32 r_const, f32 t_const) :
	ambient_coef(a_coef), specular_coef(s_coef), diffuse_coef(d_coef), 
	specular_exp(s_exp), reflection_const(r_const), refraction_const(t_const)
{

}
Phong::Phong(void)
	: IlluminationModel(0.0, 0.0, 0.0, 0.0, 0.0, 0.0), ambient_mat(0.0), 
	diffuse_mat(0.0), specular_mat(0.0)
{

}

Phong::Phong(f32 a_coef, f32 s_coef, f32 d_coef, f32 s_exp, f32 r_const, f32 t_const, glm::vec4 aM, 
		glm::vec4 dM, glm::vec4 sM) :
	IlluminationModel(a_coef, s_coef, d_coef, s_exp, r_const, t_const), 
	ambient_mat(aM), diffuse_mat(dM), specular_mat(sM)
{

}

Phong::~Phong(void)
{

}


glm::vec4 Phong::illuminate(IntersectData &id, Light light, glm::vec3 eyepoint,
		bool in_shadow)
{
	glm::vec4 surfToLight = glm::normalize(light.position - id.intersection);
	glm::vec4 viewVec = glm::normalize(-id.ray.direction);
	glm::vec4 reflectionVec = glm::normalize(glm::reflect(-surfToLight, id.normal));

	glm::vec4 ambient, diffuse, specular;
	glm::vec4 ret_col;
	
	ambient = light.ambient * ambient_coef * ambient_mat;
	if (in_shadow) {
		return ambient;
	}
	else {
		diffuse = light.color * diffuse_coef * diffuse_mat * 
			fmaxf(0.0, glm::dot(id.normal, surfToLight));
		specular = light.color * specular_coef * specular_mat *
			powf(fmaxf(glm::dot(reflectionVec, viewVec), 0.0), specular_exp);
		return ambient + diffuse + specular;
	}
}

CheckerBoard::CheckerBoard(f32 a_coef, f32 s_coef, f32 d_coef, f32 s_exp, 
		f32 r_const, f32 t_const, glm::vec4 &c1, glm::vec4 &c2) :
	IlluminationModel(a_coef, s_coef, d_coef, s_exp, r_const, t_const), 
	color1(c1), color2(c2)
{
	
}

CheckerBoard::~CheckerBoard(void)
{

}

glm::vec4 CheckerBoard::illuminate(IntersectData &id, Light light, glm::vec3 eyepoint,
		bool in_shadow)
{
	glm::vec4 surfToLight = glm::normalize(light.position - id.intersection);
	glm::vec4 viewVec = glm::normalize(glm::vec4(eyepoint, 1.0) - id.intersection);
	glm::vec4 reflectionVec = glm::normalize(glm::reflect(-surfToLight, id.normal));

	glm::vec4 ambient, diffuse, specular;
	
	ambient = light.ambient * ambient_coef * get_cube(id);

	if (in_shadow) {
		return ambient;
	}
	else {
		diffuse = light.color * diffuse_coef * get_cube(id) * 
			fmaxf(0.0, glm::dot(id.normal, surfToLight));

		specular = light.color * specular_coef * get_cube(id) *
			powf(fmaxf(glm::dot(reflectionVec, viewVec), 0.0), specular_exp);

		return ambient + diffuse + specular;
	}
}

glm::vec4 CheckerBoard::get_cube(IntersectData &id)
{
	f32 u_barry = id.u_coord;
	f32 v_barry = id.v_coord;
	f32 w_barry = 1 - u_barry - v_barry;
	glm::vec4 A(10.0, 1.0, 0.0, 0.0); // = id.triangle_points[0];
	glm::vec4 B(0.0, 1.0, 0.0, 0.0); //= id.triangle_points[1];
	glm::vec4 C(0.0, 1.0, 10.0, 0.0); //= id.triangle_points[2];

	f32 u = (u_barry * A + v_barry * B + w_barry * C).x; 
	f32 v = (u_barry * A + v_barry * B + w_barry * C).y; 



	//printf("%f %f %f\n", u_barry, v_barry, w_barry);
	u32 row_odd = ((u32)(u_barry * 12.0)) % 2;
	u32 col_odd = ((u32)(v_barry * 12.0)) % 2;
	//printf("row %d col %d\n", row_odd , col_odd);
	if (!row_odd && !col_odd) {
		return color2;
	}
	else if (!row_odd && col_odd) {
		return color1;
	}
	else if (row_odd && !col_odd) {
		return color1;
	}
	else { //(!row_odd && !col_odd) 
		return color2;
	}

}
