#include "pipeline.h"

// TODO: move into pipeline.cpp

// Edges for clipping
#define TOP 	0
#define LEFT 	1
#define BOTTOM 	2
#define RIGHT 	3

/**
 * Container for the clipping region.
 */
struct Rectf 
{
	glm::vec2 bl;	// bottom left vertex
	glm::vec2 tr;	// top right vertex
	
	/**
	 * Constructor
	 *
	 * @param ll: The lower left vertex of the clipping region.
	 * @param ur: The upper right vertex of the clippign region.
	 */
	Rectf(glm::vec2 ll, glm::vec2 ur) : bl(ll), tr(ur) {}
	
	/**
	 * Destructor.
	 */
	~Rectf() {}
};

// Local functions
local std::vector<glm::vec4> shpc(std::vector<glm::vec4> inV, Rectf win, 
									int edge);
local f32 getSlope(glm::vec4 p1, glm::vec4 p2);
local bool inside(glm::vec4 p, Rectf win, int edge);
local void intersect(glm::vec4 p1, glm::vec4 p2, Rectf win, glm::vec4 &i, int edge);

/**
 * Implementation of the Sutherland-Hodgman clipping algorithm.
 *
 * @param inV: The list of vertices to be clipped.
 * @param out: The list of modified verticies.
 * @param inSize: The size of inV.
 * @param outSize: The size of out (will be modified).
 * @param win: The clipping region.
 * @param edge: The edge that will be clipped against.
 */
local std::vector<glm::vec4> shpc(std::vector<glm::vec4> inV, Rectf win, 
		int edge) 
{
	std::vector<glm::vec4> out;
	if (inV.empty()) {
        return inV;
    }
    glm::vec4 p = inV.back();//[inSize - 1];
	glm::vec4 t;
	for (u64 i = 0; i < inV.size(); i++) {
		glm::vec4 s = inV[i];
		if (inside(s, win, edge)) {
			if (inside(p, win, edge)) {
				out.push_back(s);
				//out[outSize] = s;
				//outSize++;
			}
			else {
				intersect(p, s, win, t, edge);
				out.push_back(t);
				//out[i] = t;
				//out[outSize] = t;
				//outSize++;
				out.push_back(s);
				//out[outSize] = s;
				//outSize++;
			}
		}
		else {
			if (inside(p, win, edge)) {
				intersect(s, p, win, t, edge);
				out.push_back(t);
				//out[outSize] = t;
				//outSize++;
			}
		}
		p = s;
	}

	return out;
}

/**
 * Calculates the slope of the given line.
 *
 * @param p1: The starting point of the line.
 * @param p2: The ending point of the line.
 * @return: The slope of the line, will be zero if delta-x or delta-y is 
 * found to be zero.
 */
local f32 getSlope(glm::vec4 p1, glm::vec4 p2) 
{
	f32 y = p2.y - p1.y;
	f32 x = p2.x - p1.x;
	if (x == 0 || y == 0)
		return 0;
	return y / x;
}

/**
 * Determines if a point is within the clipping region.
 *
 * @param p: The point to be checked.
 * @param win: The clipping region.
 * @param edge: The edge that is checked against.
 */
local bool inside(glm::vec4 p, Rectf win, int edge) 
{
	if (edge == TOP) {
		return p.y <= win.tr.y;
	}
	else if (edge == LEFT) {
		return p.x >= win.bl.x;
	}
	else if (edge == BOTTOM) {
		return p.y >= win.bl.y;
	}
	else {	// RIGHT
		return p.x <= win.tr.x;
	}
}

/**
 * Calculates the intersection of a given line with the given edge.
 *
 * @param p1: The point of the line that is outside the edge.
 * @param p2: The point of the line that is inside the edge.
 * @param win: The clipping region.
 * @param i: The intersection point.
 * @param edge: The edge that is used in calculating the intersection.
 */
local void intersect(glm::vec4 p1, glm::vec4 p2, Rectf win, glm::vec4 &i, 
				int edge) 
{
	f32 m, b;
	m = getSlope(p1, p2);
	b = p2.y - m * p2.x;
	i.z = p1.z;
	i.w = p1.w;

	if (edge == TOP ) {
		i.y = win.tr.y;
		if (m != 0)	
			i.x = (i.y - b) / m;
		else
			i.x = p2.x;
	}
	else if (edge == BOTTOM ) {
		i.y = win.bl.y;
		if (m != 0)
			i.x = (i.y - b) / m;
		else
			i.x = p2.x;
	}
	else if (edge == LEFT ) {
		i.x = win.bl.x;
		i.y = m * i.x + b;
	}
	else if (edge == RIGHT) {	// RIGHT
		i.x = win.tr.x;
		i.y = m * i.x + b;
	}
	else {
		i.x = p1.x;
		i.y = p1.y;
	}
}

std::vector<glm::vec4> clipPolygon(std::vector<glm::vec4> &poly, glm::vec2 ll, glm::vec2 ur) 
{
	std::vector<glm::vec4> ret;
	Rectf rect(ll, ur);
	for (u64 i = 0; i < poly.size(); i += 3) {
		std::vector<glm::vec4> in;
		in.push_back(poly[i]);
		in.push_back(poly[i + 1]);
		in.push_back(poly[i + 2]);
		// TODO: instead of vectors use static arrays once you've figured out
		// the biggest polygon that can result from clipping a triangle
	//	printf("start %ld\n", poly.size());
		std::vector<glm::vec4> out1, out2, out3, out4;
		out1 = shpc(in, rect, TOP);
		out2 = shpc(out1, rect, LEFT);
		out3 = shpc(out2, rect, BOTTOM);
		out4 = shpc(out3, rect, RIGHT);
	//	printf("end %ld\n", out4.size());
	
		// TODO: This is slow and stupid, fix it.
		for (u32 j = 0; j < out4.size(); j++) {
			ret.push_back(out4[j]);
		}
	}

	return ret;	
}



