/**
 * FILE: obj.h
 * Author: James Moir (jgm6103)
 *
 * Contains a function which will read an obj file and place it's contents into
 * a canvas for later use.
 *
 * All obj files were obtained by exporting a model created in blender.
 */
#ifndef OBJ_H
#define OBJ_H

#include "utils.h"
#include "pipeline.h"

// TODO: Move into a utils file

/**
 * Reads the obj file at the given address and adds the contents to the given
 * Canvas object.
 *
 * @param objSrc: The obj source file.
 * @param can: The Canvas that the obj file contents will be added to.
 */
VBO readOBJ(const char *objSrc);
#endif
