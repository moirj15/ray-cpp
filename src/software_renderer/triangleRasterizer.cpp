#include "pipeline.h"
#include <algorithm>

//TODO: move into pipeline.cpp

/**
 * Draws the given VBO to the given canvas.
 *
 * @param vbo: The vbo that will be drawn to the canvas.
 * @param can: The canvas that will be drawn to.
 * @param values: The color values for each triangle.
 */
static inline
void fill_triangle(glm::vec4 triangle[3], Canvas *can, u32 value)
{
    glm::vec4 v0 = triangle[0];
    glm::vec4 v1 = triangle[1];
    glm::vec4 v2 = triangle[2];
    if (v0.y > v1.y) {
        std::swap(v0, v1);
    }
    if (v1.y > v2.y) {
        //printf("swap\n");
        std::swap(v1, v2);
    }
    if (v0.y > v1.y) {
        std::swap(v0, v1);
    }
    f32 m0, m1, m2, z0, z1, z2;
    if (v1.y - v0.y != 0.0f) {
        m0 = (v1.x - v0.x) / (v1.y - v0.y);
        z0 = (v1.z - v0.z) / (v1.y - v0.y);
    }
    else {
        m0 = 0.0f;
        z0 = 0.0f;
    }
    if (v2.y - v0.y != 0.0f) {
        m1 = (v2.x - v0.x) / (v2.y - v0.y);
        z1 = (v2.z - v0.z) / (v2.y - v0.y);
    }
    else {
        m1 = 0.0f;

        z2 = 0.0f;
    }
    if (v2.y - v1.y != 0.0f) {
        m2 = (v2.x - v1.x) / (v2.y - v1.y);
        z2 = (v2.z - v1.z) / (v2.y - v1.y);
    }
    else {
        m2 = 0.0f;

        z2 = 0.0f;
    }
    f32 start_x = v0.x;
    f32 end_x = v0.x;
    f32 start_z = v0.z;
    f32 end_z = v0.z;
    u32 y = (u32)v0.y;

    // m0 is the line on the right half of the triangle
    if (m0 > m1) {
        while (y < (u32)v1.y) {
            can->fill_row(start_x, end_x, start_z, end_z, y, value);
            start_x += m1;
            end_x += m0;
            start_z += z1;
            end_z += z0;
            y++;
        }
        end_x = v1.x;
        end_z = v1.z;
        while (y < (u32)v2.y) {
            can->fill_row(start_x, end_x, start_z, end_z, y, value);
            start_x += m1;
            end_x += m2;
            start_z += z1;
            end_z += z2;
            y++;
        }
    }
    else {
        while (y < (u32)v1.y) {
            can->fill_row(start_x, end_x, start_z, end_z, y, value);
            start_x += m0;
            end_x += m1;
            start_z += z0;
            end_z += z1;
            y++;
        }
        start_x = v1.x;
        start_z = v1.z;
        while (y < (u32)v2.y) {
            can->fill_row(start_x, end_x, start_z, end_z, y, value);
            start_x += m2;
            end_x += m1;
            start_z += z2;
            end_z += z1;
            y++;
        }
    }
}



void draw_vbo(VBO vbo, Canvas *can, std::vector<u32> values)
{
    //printf("%ld\n", vbo.connections.size());
    //sort_triangles(vbo);
    for (u64 i = 0; i < vbo.connections.size(); i += 3) {
        // Triangle rasterizer is buggy

        
        glm::vec4 triangle[3] = {
            vbo.vertexList[vbo.connections[i]],
            vbo.vertexList[vbo.connections[i + 1]],
            vbo.vertexList[vbo.connections[i + 2]]
        };
        fill_triangle(triangle, can, values[i / 3]);
        /*
        std::vector<glm::vec2> tmp = {
            glm::vec2(vbo.vertexList[vbo.connections[i]]),
            glm::vec2(vbo.vertexList[vbo.connections[i + 1]]),
            glm::vec2(vbo.vertexList[vbo.connections[i + 2]]),
        };
        fillPolygon(tmp, can, values[i / 3]); 
    */}
}
