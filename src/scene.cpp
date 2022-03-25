#include "scene.h"
#include "intersectData.h"
#include "ray.h"

Object *Scene::CastRay(const Ray &ray, IntersectData &data, s32 check_obj) const
{
}

void Scene::transform(glm::mat4 mat)
{
    for (const auto &object : objList) {
        object->Transform(mat);
    }
	for (u64 l = 0; l < lightList.size(); l++) {
		lightList[l].position = mat * glm::vec4(lightList[l].position, 1.0); 
	}
}
