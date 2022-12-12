#include "pipeline.h"

/**
 * Converts the given polygon into a VBO filled with triangles.
 *
 * @param polygon: The polygon that will be converted.
 * @return: The VBO containing the polygons.
 */
VBO triangulate(std::vector<glm::vec4> polygon)
{
    VBO ret;
    ret.vertexList = polygon;
    u64 vert_count = polygon.size();
    u64 clip_count = 0;
    u64 i = 0;

    while (vert_count - clip_count >= 3) {
        u64 left, right;
        if (i == 0) {
            left = vert_count - 1;
            right = 1;
        }
        else if (i == vert_count - 1) {
            left = i - 1;
            right = 0;
        }
        else {
            left = i - 1;
            right = i + 1;
        }
        ret.connections.push_back(left);
        ret.connections.push_back(i);
        ret.connections.push_back(right);
        i += 2;
        clip_count++;
    }

#if 0
    printf("tris\n");
    printf("size %ld\n", ret.connections.size());
    for (u64 i = 0; i < ret.connections.size(); i++) {
        printf("%s\n", glm::to_string(ret.vertexList[ret.connections[i]]).c_str());
    }
#endif
    return ret;
}
