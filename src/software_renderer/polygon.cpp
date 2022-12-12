#include "polygon.h"

// TODO: Delete this file

/**
 * Constructor.
 */
Polygon::Polygon() 
{

}

/**
 * Constructor.
 *
 * @param p: The list of points.
 * @param c: The list of connectivity data.
 */
Polygon::Polygon(std::vector<glm::vec2> p, std::vector<u64> c) 
		: points(p), connections(c) 
{

}

/**
 * Destructor.
 */
Polygon::~Polygon(void) 
{

}


void Polygon::print(void)
{
	for (u64 i = 0; i < points.size(); i++) {
		printf("p %ld | (%f, %f)\n", i, points[i].x, points[i].y);
	}
}
