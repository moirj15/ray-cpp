#ifndef VBO_H
#define VBO_H

#include "utils.h"

struct VBO
{
    std::vector<glm::vec4> 	vertexList;
    std::vector<u64>		connections;

	VBO(void);
    VBO(std::vector<glm::vec4> v, std::vector<u64> c);

    ~VBO(void);
};

#endif
