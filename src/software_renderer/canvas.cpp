#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "canvas.h"

/**
 * Constructor. 
 *
 * @param w: The width.
 * @param h: The height.
 */
Canvas::Canvas(u32 w, u32 h) : m_width(w), height(h) {
	pixels = new u32[m_width * height];
    back_buffer = new u32[m_width * height];
    depth_buffer = new f32[m_width * height];
}

/**
 * Destructor.
 */
Canvas::~Canvas(void) {
    delete[](pixels);
    delete[](back_buffer);
    delete[](depth_buffer);
}

/**
 * Clears the canvas.
 *
 * @param can: The canvas object that will be cleared.
 */
void Canvas::clearCanvas(void) {
    u32 *p = pixels;
    pixels = back_buffer;
    back_buffer = p;
    //memset(pixels, 0, width * height * sizeof(u32));
    memset(back_buffer, 0, m_width * height * sizeof(u32));
    //memset(depth_buffer, 0, width * height * sizeof(f32));
    s32 len = m_width * height;
    f32 *tmp = depth_buffer;
    while (len-- > 0) {
        *tmp++ = INFINITY;
    }
}

/**
 * Sets the pixel at the given coordinates to the given value.
 *
 * @param can: The Canvas that will have a pixel set.
 * @param x: The x coordinate.
 * @param y: The y coordinate.
 * @param value: The value that the pixel will be set to.
 */
void Canvas::setCanvasPixel(u32 x, u32 y, u32 value) {
    //can->pixels[((can->width * can->height) - 1)* x + y] = value;
    y = height - y;
    back_buffer[(m_width) * y + x] = value;
}

/**
 * Fills a row of the canvas with the given color value.
 *
 * @param start_x: The starting x position.
 * @param end_x: The ending x position.
 * @param y: The y value.
 * @param value: The value to fill the canvas with.
 */
void Canvas::fill_row(u32 start_x, u32 end_x, f32 start_z, f32 end_z, u32 y, u32 value)
{
    s32 len, s, e;
    f32 e_z, s_z;
    if (start_x > end_x) {
        len = start_x - end_x;
        s = end_x;
        e = start_x;
        s_z = end_z;
        e_z = start_z;
    }
    else {
        len = end_x - start_x;
        s = start_x;
        e = end_x;
        s_z = start_z;
        e_z = end_z;
    }
    
    u32 *row = &(back_buffer[(m_width) * (height - y) + s]);
    f32 *zs = &(depth_buffer[(m_width) * (height - y) + s]);
    //u32 *tmp = row;
    //memset(row, 0, len);
    f32 depth = s_z + ((end_z - start_z) / (f32)len) * (ceil(start_z) - start_z);
    while (len-- > 0) {
        if (depth < *zs) {
            *zs = depth;
            *row = value;
        }
        depth += (e_z - s_z) / (f32)len;
        //printf("%X\n", *row);
        row++;
        zs++;
    }
}
