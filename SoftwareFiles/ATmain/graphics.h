

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "Colours.h"
#include "Fill.h"
#include "OutGraphicsChar.h"

#define XRES	800
#define YRES	480

/*
 * Writes a pixel at coordinate (x, y)
 */

void WriteAPixel(int x, int y, int colour);

/* 
 * programs an index in the colour palette LUT with the value in RGB 
 */
 
void ProgramPalette(int PaletteNumber, int RGB);


/*
 * reads a pixel at coordinate (x, y)
 */

int ReadAPixel(int x, int y);



/* 
 * draws a horizontal line starting at pixel (x,y). 
 * Implemented in hardware
 */
void HLine(int x, int y, int length, int colour);


/*
 * draws a vertical line starting at pixel (x,y)
 * implemented in hardware
 */

void VLine(int x1, int y1, int length, int colour);

/*
 * Draws an unfilled rectangle
 * (x1, y1) is the upper left corner of the rectangle
 * (x2, y2) is the bottom right corner
 */

void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int colour);


void Rectangle(int x1, int y1, int x2, int y2, int colour);


void DrawString(int x1, int y1, char* string, int strlen, int fontcolour, int backgroundcolour);


void DrawButton(int x1, int y1, int x2, int y2, char* string, int strlen, int fontcolour, int backgroundcolour);

void TriangleTest(void);


/*
 * Draws a line from pixel coordinate (x1, y1) to (x2, y2)
 * implemented in hardware using bresenham's algorithm
 */

void Line(int x1, int y1, int x2, int y2, int colour);


#endif


