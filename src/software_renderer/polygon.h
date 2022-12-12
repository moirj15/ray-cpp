#ifndef POLYGON_H
#define POLYGON_H

#include "utils.h"

// TODO: Delete this file


struct Polygon {
	std::vector<glm::vec2> 	points;		
	std::vector<u64> 		connections;

	/**
	 * Constructor.
	 */
	Polygon(void);

	/**
	 * Constructor.
	 *
	 * @param p: The list of points.
	 * @param c: The list of connectivity data.
	 */
	Polygon(std::vector<glm::vec2> p, std::vector<u64> c);

	/**
	 * Destructor.
	 */
	~Polygon(void);

	void print(void);
};

#endif 
