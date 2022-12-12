#ifndef CANVAS_H
#define CANVAS_H

#include "utils.h"

/**
 * Canvas for drawing to.
 */
struct Canvas {
    u32 m_width;     // the width of the canvas
    u32 height;    // the height of the canvas
    u32 *pixels;   // the array of pixel data
    u32 *back_buffer;
    f32 *depth_buffer;
		
	/**
 	 * Constructor. 
 	 *
 	 * @param w: The width.
 	 * @param h: The height.
 	 */
	Canvas(u32 x, u32 h);
	
	/**
	 * Destructor.
	 */
	~Canvas(void);

	/**
	 * Clears the canvas.
	 *
	 * @param can: The canvas object that will be cleared.
	 */
	void clearCanvas(void);

	/**
	 * Sets the pixel at the given coordinates to the given value.
	 *
	 * @param can: The Canvas that will have a pixel set.
	 * @param x: The x coordinate.
	 * @param y: The y coordinate.
	 * @param value: The value that the pixel will be set to.
	 */
	void setCanvasPixel(u32 x, u32 y, u32 value);

	/**
     * Fills a row of the canvas with the given color value.
     *
     * @param start_x: The starting x position.
     * @param end_x: The ending x position.
     * @param y: The y value.
     * @param value: The value to fill the canvas with.
     */
    void fill_row(u32 start_x, u32 end_x, f32 start_z, f32 end_z, u32 y, u32 value);

};



#endif
