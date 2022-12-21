#include "pipeline.h"

namespace ra
{

// TODO: move into pipeline.cpp

/**
 * Transforms the given VBO into a set of 2D triangles and lights them.
 *
 * @param object: The VBO that will be transformed.
 * @param camera: The camera viewing the object.
 * @param light: The scene's light.
 * @param mat: The material of the object.
 * @param colors: The list of colors that will be filled after lighting the
 * triangles.
 * @return: The list of 2D triangles.
 */
VBO transform_to_2D(VBO object, Camera *cam, const Light &light, const Material &mat, std::vector<u32> &colors)
{
    VBO vbo, ret;
    // std::vector< std::vector<glm::vec2> > ret;
    std::vector<glm::vec4> &points      = object.vertexList;
    std::vector<u64>       &connections = object.connections;

    glm::vec2 ll(0.0, 0.0);
    glm::vec2 ur(639.0, 479.0);

    // construct matrix for converting to screen space
    // TODO: place this in the constructor.
    glm::mat4 screenSpace(1.0f);

    screenSpace[0][0] = 320.0;
    screenSpace[3][0] = 320.0;
    screenSpace[1][1] = 240.0;
    screenSpace[3][1] = 240.0;
    for (u64 i = 0; i < connections.size(); i += 3) {
        glm::vec4 p0 = points[connections[i]];
        glm::vec4 p1 = points[connections[i + 1]];
        glm::vec4 p2 = points[connections[i + 2]];
#if 0
		printf("\npoints\n");
		printf("%s\n", glm::to_string(p0).c_str());
		printf("%s\n", glm::to_string(p1).c_str());
		printf("%s\n", glm::to_string(p2).c_str());
#endif
        glm::vec4 tmp0 = cam->projection * cam->cameraMat * cam->modelMat * p0;
        glm::vec4 tmp1 = cam->projection * cam->cameraMat * cam->modelMat * p1;
        glm::vec4 tmp2 = cam->projection * cam->cameraMat * cam->modelMat * p2;

#if 0
		printf("transformed \n");
		printf("%s\n", glm::to_string(tmp0).c_str());
		printf("%s\n", glm::to_string(tmp1).c_str());
		printf("%s\n", glm::to_string(tmp2).c_str());
#endif

        std::vector<glm::vec4> tri;

        tri.push_back(tmp0);
        tri.push_back(tmp1);
        tri.push_back(tmp2);
        u32       color   = phong_light(tri, light, mat);
        glm::vec4 t_vert0 = screenSpace * tmp0;
        glm::vec4 t_vert1 = screenSpace * tmp1;
        glm::vec4 t_vert2 = screenSpace * tmp2;

        t_vert0.x = t_vert0.x / t_vert0.w;
        t_vert0.y = t_vert0.y / t_vert0.w;
        t_vert0.z = t_vert0.z / t_vert0.w;

        t_vert1.x = t_vert1.x / t_vert1.w;
        t_vert1.y = t_vert1.y / t_vert1.w;
        t_vert1.z = t_vert1.z / t_vert1.w;

        t_vert2.x = t_vert2.x / t_vert2.w;
        t_vert2.y = t_vert2.y / t_vert2.w;
        t_vert2.z = t_vert2.z / t_vert2.w;

#if 0
		printf("2d\n");
		printf("%s\n", glm::to_string(t_vert0).c_str());
		printf("%s\n", glm::to_string(t_vert1).c_str());
		printf("%s\n", glm::to_string(t_vert2).c_str());
#endif

        std::vector<glm::vec4> triangle;

        triangle.push_back(t_vert0);
        triangle.push_back(t_vert1);
        triangle.push_back(t_vert2);

        std::vector<glm::vec4> clipped = clipPolygon(triangle, ll, ur);
        std::vector<glm::vec2> d2;

        if (clipped.size() > 3) {
            vbo = triangulate(clipped);

            // colors.push_back(colors.back());
        } else {
            vbo.vertexList       = clipped;
            u64 connections_size = vbo.connections.size();
            vbo.connections.push_back(0);
            vbo.connections.push_back(1);
            vbo.connections.push_back(2);
        }
        u64 vert_size = ret.vertexList.size();
        for (u64 i = 0; i < vbo.connections.size(); i += 3) {
            ret.connections.push_back(vbo.connections[i] + ret.vertexList.size());
            ret.connections.push_back(vbo.connections[i + 1] + ret.vertexList.size());
            ret.connections.push_back(vbo.connections[i + 2] + ret.vertexList.size());
            colors.push_back(color);
        }
        for (u64 i = 0; i < vbo.vertexList.size(); i++) {
            ret.vertexList.push_back(vbo.vertexList[i]);
        }

        /*
        for (u64 c = 0; c < clipped.size(); c++) {
                d2.push_back(glm::vec2(clipped[c]));
}*/
        // ret.push_back(d2);
        // f32 x, y;
        // printf("%s\n", glm::to_string(t).c_str());
        // printf("(%f, %f)\n", x, y);
        // ret.push_back(glm::vec2(x, y));
    }
    return ret;
}
} // namespace ra