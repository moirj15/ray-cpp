#include "vbo.h"


VBO::VBO(void)
{
    
}

VBO::VBO(std::vector<glm::vec4> v, std::vector<u64> c)
	: vertexList(v), connections(c)
{
    
}

VBO::~VBO(void)
{
    
}
