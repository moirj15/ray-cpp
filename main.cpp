#include <cstdio>
#include <cstdlib>

#include "utils.h"
#include "bmpIO.h"
#include "world.h"
#include "object.h"
#include "camera.h"
#include "illumination.h"

static const Light light(glm::vec4(0.97f, 3.00f, 9.5f, 1.0f),
                        glm::vec4(100.0f, 100.0f, 100.0f, 1.0f),
                        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

static const glm::vec3 camera_pos(1.67f, -1.28f, 7.37f);
static const glm::vec3 look_at(0.0f, 0.0f, -1.0f);
static const glm::vec3 up_vec(0.0f, 1.0f, 0.0f);

static const glm::vec4 ambient_mat( 1.0, 1.0, 1.0, 1.0);
static const glm::vec4 diffuse_mat( 1.0, 1.0, 1.0, 1.0);
static const glm::vec4 specular_mat(1.0, 1.0, 1.0, 1.0);

static const f32 film_plane_width = 16.0f;
static const f32 film_plane_height = 10.0f;
static const f32 ldmax = 1.0f;

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

  	World world;

	world.lightList.push_back(light);

	Phong sphere_illum1(0.0, 0.5, 0.5, 70.0, 0.0, 0.8, ambient_mat, diffuse_mat, 
			specular_mat);
	Phong sphere_illum2(0.0, 0.5, 0.5, 70.0, 0.5, 0.0, ambient_mat, diffuse_mat, 
			specular_mat);

	glm::vec4 color1(1.0, 0.0, 0.0, 0.0);
	glm::vec4 color2(1.0, 1.0, 0.0, 0.0);

	CheckerBoard floor_illum(0.5, 0.5, 0.5, 70.0, 0.0, 0.0, color1, color2);

	world.objList.push_back(new Sphere(glm::vec4(1.49, -1.31, 5.70, 1.0), 1.00,
							&sphere_illum1));

    std::vector<glm::vec4> triangles;

    triangles.push_back(glm::vec4(15.8, -1.90, 7.69, 0.0));
    triangles.push_back(glm::vec4(-4.6, -1.90, 8.69, 0.0));
    triangles.push_back(glm::vec4(-4.6, -1.90, -18.69, 0.0));

    triangles.push_back(glm::vec4(15.8, -1.90, 7.69, 0.0));
    triangles.push_back(glm::vec4(2.8,  -1.90, -19.00, 0.0));
    triangles.push_back(glm::vec4(-4.6, -1.90, -18.69, 0.0));
	
	world.objList.push_back(new Polygon(triangles, &floor_illum));

	world.objList.push_back(new Sphere(glm::vec4(2.93, -2.00, 4.21, 1.0), 0.8,
							&sphere_illum2));

	Camera *camera = new Camera(camera_pos, look_at, up_vec, film_plane_width,
                                film_plane_height, ldmax);

	world.transform(camera->camTransform);

	camera->render(world);

	writeBMP("test.bmp", camera->picture, WIDTH, HEIGHT);

    delete(camera);

	return EXIT_SUCCESS;
}
