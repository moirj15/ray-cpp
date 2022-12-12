
/**
 * FILE: obj.cpp
 * Author: James Moir (jgm6103)
 *
 * Contains a function which will read an obj file and place it's contents into
 * a canvas for later use.
 *
 * All obj files were obtained by exporting a model created in blender.
 */

#include <cstdio>
#include <cstdlib>
#include <vector>

#include "pipeline.h"

// TODO: move into a utils file

//#include "obj.h"

using namespace std;

/**
 * Reads the obj file at the given address and adds the contents to the given
 * Canvas object.
 *
 * @param objSrc: The obj source file.
 * @param can: The Canvas that the obj file contents will be added to.
 */
VBO readOBJ(const char *objSrc) {
	FILE *fp = NULL;
	fp = fopen(objSrc, "r");
	if (!fp) {
		fprintf(stderr, "error opening OBJ file %s\n", objSrc);
		exit(EXIT_FAILURE);
	}
	char *line = NULL;
	size_t len = 0;
	int read;
	glm::vec4 tmp(0.0, 0.0, 0.0, 1.0);
	vector<glm::vec4> tmpVertList;
	vector<u64> connections;
	int pointA, pointB, pointC, normA, normB, normC;
	
	while ((read = getline(&line, &len, fp)) != -1) {
		if (line[0] == 'v' && line[1] ==  ' ') {
			sscanf(line, "v %f %f %f", &tmp.x, &tmp.y, &tmp.z);
			tmpVertList.push_back(tmp);
		}
		else if (line[0] == 'v' && line[1] == 'n') {
			sscanf(line, "vn %f %f %f", &tmp.x, &tmp.y, &tmp.z);
		}
		else if (line[0] == 'f') {
			sscanf(line, "f %d//%d %d//%d %d//%d\n", 
							&pointA, &normA, &pointB, &normB, &pointC, &normC);
			pointA -= 1;
			pointB -= 1;
			pointC -= 1;
			
			normA -= 1;
			normB -= 1;
			normC -= 1;
			connections.push_back(pointA);
			connections.push_back(pointB);
			connections.push_back(pointC);
			
		}
	}
	delete[](line);	// free buffer
	fclose(fp);

	return VBO(tmpVertList, connections);
}
