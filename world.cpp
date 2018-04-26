#include "world.h"


World::World(void)
{

}

World::~World(void)
{

}

void World::transform(glm::mat4 mat)
{
	for (u64 i = 0; i < objList.size(); i++) {
		if (objList[i]->type == SPHERE) {
			Sphere *sphere = static_cast<Sphere *>(objList[i]);
			sphere->center = mat * sphere->center;
		}
        else if (objList[i]->type == POLYGON) {
            Polygon *polygon = static_cast<Polygon *>(objList[i]);
            for (u64 p = 0; p < polygon->vertices.size(); p++) {

                polygon->vertices[p] = mat * polygon->vertices[p];
            }
        }
	}
	for (u64 l = 0; l < lightList.size(); l++) {
		lightList[l].position = mat * lightList[l].position; 
	}
}
