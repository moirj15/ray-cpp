#pragma once
#include "utils.h"
#include "pipeline.h"

namespace ra
{

// TODO: Move into a utils file

/**
 * Reads the obj file at the given address and adds the contents to the given
 * Canvas object.
 *
 * @param objSrc: The obj source file.
 * @param can: The Canvas that the obj file contents will be added to.
 */
VBO readOBJ(const char *objSrc);
}
