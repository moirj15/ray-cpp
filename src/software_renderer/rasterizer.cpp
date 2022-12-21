#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "pipeline.h"
//#include "lighting.h"

//TODO: Delete this file

namespace ra
{

#define printBucket(B)      printf("yMax %d x %d dx %d dy %d sum %d index %d\n", \
                                    B.yMax, B.x, B.dx, B.dy, B.sum, B.index)
/**
 * Container for information used to fill polygons.
 */
struct Bucket {
    int     yMax;   // The max y value of a line
    int     x;      // The x value coresponding to the minimum y value
    int     dx;     // The change in x
    int     dy;     // The change in y
    int     sum;    // The sum (used for adjusting x)
    int     index;  // The index that the Bucket will be placed into in the
                    // edge table (also the y min)
    bool    pos;    // true if the slope of the line is positive, false otherwise

    /**
     * Constructor
     */
    Bucket() { }

    /**
     * Constructor
     * 
     * @param x0: The first x coordinate of the line.
     * @param y0: The first y coordinate of the line.
     * @param x1: The second x coordinate of the line.
     * @param y1: The second y coordinate of the line.
     */
    Bucket(int x0, int y0, int x1, int y1) {
        int sdx, sdy;   // signed dx and dy
        if (y0 < y1) {
            yMax  = y1;
            x     = x0;
            sdx   = x1 - x0;
            sdy   = y1 - y0;
            index = y0;
        }
        else {
            yMax  = y0;
            x     = x1;
            sdx   = x0 - x1;
            sdy   = y0 - y1;
            index = y1;
        }
        pos = ((sdx > 0 && sdy > 0) || (sdx < 0 && sdy < 0));
        dx =  abs(sdx);
        dy =  abs(sdy);
        sum = 0;
    }

    /**
     * Destructor
     */
    ~Bucket() { }

    /**
     * Operator overload for "<", used for sorting the active list.
     */
    bool operator < (const Bucket &b) {
        if (this->x == b.x) {
            if (this->dy == b.dy) {
                return this->dx < b.dx;
            }
            return this->dy < b.dy;
        }
        return this->x < b.x;
    }
};


// Local Functions
local void drawPolygon(int n, const int x[], const int y[], u32 value
						, Canvas *can);
local void initEdgeTable(std::vector<std::vector<Bucket> > &et, int n, 
							const int x[], const int y[]);
local void purge(std::vector<Bucket> &buck, int y);
local void fillScan(std::vector<Bucket> al, int y, u32 value, Canvas *can);
local void merge(std::vector<Bucket> &a, const std::vector<Bucket> b);
local void adjust(std::vector<Bucket> &al); 

/**
 * Fill the given polygon onto the given canvas with the given color.
 *
 * @param poly: The polygon that will be filled.
 * @param can: The canvas that will be drawn to.
 * @param value: The color that the polygon will be filled with.
 */
void fillPolygon(std::vector<glm::vec2> poly, Canvas *can, u32 value) {
	if (poly.empty())
		return;
	std::vector<s32> x;
	std::vector<s32> y;
	//s32 *x = new s32[poly.size()];
	//s32 *y = new s32[poly.size()];

	for (u64 i = 0; i < poly.size(); i++) {
		// extract the x and y components
		x.push_back((s32)poly[i].x);
		y.push_back((s32)poly[i].y);
		//x[i] = (s32)poly[i].x;
		//y[i] = (s32)poly[i].y;
		// draw the polygon
	}
	drawPolygon(poly.size(), x.data(), y.data(), value, can);
	//delete[](x);
	//delete[](y);
}

/**
 * Runs the polygon fill algorithm.
 *
 * @param n: The number of points.
 * @param x: The x component.
 * @param y: The y component.
 * @param value: The value used to fill the polygon.
 * @param can: The canvas that the polygon will be drawn to.
 */
local void drawPolygon(int n, const int x[], const int y[], u32 value, 
		Canvas *can) {
	std::vector<std::vector<Bucket> > edgeTable(can->height);
	std::vector<Bucket> activeList;
    initEdgeTable(edgeTable, n, x, y);
    u32 yCurr = 0;

    while (edgeTable[yCurr].size() == 0) {
        yCurr++;
    }
    while (yCurr < can->height) {
        if (!edgeTable[yCurr].empty()) {
            merge(activeList, edgeTable[yCurr]);
        }
        purge(activeList, yCurr); 
        sort(activeList.begin(), activeList.end());
        fillScan(activeList, yCurr, value, can);
        yCurr++;
        adjust(activeList);
    }
}

/**
 * Initializes and populates the edge table.
 * 
 * @param et: Reference to the edge table that will be initialized.
 * @param n: The number of verticies that will be used to initialize the 
 * edge table.
 * @param x: The list of x verticies.
 * @param y: THe list of y verticies.
 */
local void initEdgeTable(std::vector<std::vector<Bucket> > &et, int n, 
		const int x[], const int y[]) {
    for (int i = 0; i < n; i++) {
        Bucket item;
        if (i == n - 1) {
            item = Bucket(x[i], y[i], x[0], y[0]);
        }
        else {
            item = Bucket(x[i], y[i], x[i + 1], y[i + 1]);
        }
        et[item.index].push_back(item);        
    }
}

/**
 * Removes all buckets from buck where yMax == y.
 * 
 * @param buck: Reference to the list that will have the buckets meeting
 * the requirements removed.
 * @param y: The value used to determine which buckets will be removed.
 */
local void purge(std::vector<Bucket> &buck, int y) {
    if (buck.empty()) {
        return;
	}
	std::vector<int> remove;
    for (u64 i = 0; i < buck.size(); i++) {
        if (buck[i].yMax <= y) {
            remove.push_back(i);
        }
    }
    if (remove.size() == 0) {
        return;
    }
    for (s64 i = remove.size() - 1; i >= 0; i--) {
        buck.erase(buck.begin() + remove[i]);
    }
    //buck.shrink_to_fit();
}

/**
 * Fills the scan line using the given active list.
 * 
 * @param al: The active list used to fill lines.
 * @param y: The scan line that will be filled.
 */
local void fillScan(std::vector<Bucket> al, int y, u32 value, Canvas *can) {
    for (u64 i = 0; i < al.size(); i++) {
        Bucket a, b;
        a = al[i];
        i++;
        if (i >= al.size()) {
            return;
        }
        b = al[i];
        for (int j = a.x; j < b.x; j++) {
            can->setCanvasPixel(j, y, value);
        }
    }
}

/**
 * Merges two vectors into a single vector.
 *
 * @param a: Reference to the vector that will be added to.
 * @param b: The vector that will be merged with a.
 */
local void merge(std::vector<Bucket> &a, const std::vector<Bucket> b) {
    for (u64 i = 0; i < b.size(); i++) {
        a.push_back(b[i]);
    }
}

/**
 * Adjusts the x values in the active list.
 * 
 * @param al: Reference to the active list that will be adjusted.
 */
void adjust(std::vector<Bucket> &al) {
    for (u64 i = 0; i < al.size(); i++) {
        al[i].sum += al[i].dx;
        if (al[i].dy == 0) {
            continue;
        }
        while (al[i].sum >= al[i].dy) {
            if (al[i].pos) {
                al[i].x++;
            }
            else {
                al[i].x--;
            }
            al[i].sum -= al[i].dy;
        }
    }
}

}