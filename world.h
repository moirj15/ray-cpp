#ifndef WORLD_H
#define WORLD_H

#include "utils.h"
#include "object.h"

struct World
{
	std::vector<Object *> 	objList;
	std::vector<Light>		lightList;

	World(void);
	~World(void);

	void transform(glm::mat4 mat);
};

#endif
