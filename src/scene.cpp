#include "Scene.h"


void Scene::transform(glm::mat4 mat)
{
    for (const auto &object : objList) {
        object->Transform(mat);
    }
	for (u64 l = 0; l < lightList.size(); l++) {
		lightList[l].position = mat * glm::vec4(lightList[l].position, 1.0); 
	}
}
