/*
 * graphics.c
 *
 * graphics library to draw lines, shapes, and text to LCD screen
 *
 *  Created on: Jan 29, 2016
 *      Author: Mitchell Nichols
 */

#include <stdio.h>
#include "graphics.h"


/* graphics controller register definitions */
#define GraphicsCommandReg				(*(volatile unsigned short int *)(0x84000000))
#define GraphicsStatusReg				(*(volatile unsigned short int *)(0x84000000))
#define GraphicsX1Reg					(*(volatile unsigned short int *)(0x84000002))
#define GraphicsY1Reg					(*(volatile unsigned short int *)(0x84000004))
#define GraphicsX2Reg					(*(volatile unsigned short int *)(0x84000006))
#define GraphicsY2Reg					(*(volatile unsigned short int *)(0x84000008))
#define GraphicsColourReg				(*(volatile unsigned short int *)(0x8400000E))
#define GraphicsBackgroundColourReg		(*(volatile unsigned short int *)(0x84000000))


/* macro to wait until graphics controller is in idle state */
#define WAIT_FOR_GRAPHICS				while((GraphicsStatusReg & 0x0001) != 0x0001);

/* graphics command definitions */
#define DrawHLine						1
#define DrawVLine						2
#define Drawline						3
#define PutAPixel						0xA
#define GetAPixel						0xB
#define ProgramPaletteColour			0x10


/*
 * Writes a pixel at coordinate (x, y)
 */

void WriteAPixel(int x, int y, int colour)
{
	WAIT_FOR_GRAPHICS;
	GraphicsX1Reg = x;
	GraphicsY1Reg = y;
	GraphicsColourReg = colour;
	GraphicsCommandReg = PutAPixel;
}


/*
 * reads a pixel at coordinate (x, y)
 */

int ReadAPixel(int x, int y)
{
	WAIT_FOR_GRAPHICS;
	GraphicsX1Reg = x;
	GraphicsY1Reg = y;
	GraphicsCommandReg = GetAPixel;
	WAIT_FOR_GRAPHICS;
	return (int)(GraphicsColourReg);
}


/* 
 * programs an index in the colour palette LUT with the value in RGB 
 */ 
void ProgramPalette(int PaletteNumber, int RGB)
{
	WAIT_FOR_GRAPHICS;
	GraphicsColourReg = PaletteNumber;
	GraphicsX1Reg = RGB >> 16;
	GraphicsY1Reg = RGB;
	GraphicsCommandReg = ProgramPaletteColour;
}

/* draws a horizontal line starting at pixel (x,y). 
 * Implemented in hardware
 */
void HLine(int x, int y, int length, int colour)
{

	WAIT_FOR_GRAPHICS;
	GraphicsX1Reg = x;
	GraphicsY1Reg = y;
	GraphicsX2Reg = x+length;
	GraphicsY2Reg = y;
	GraphicsColourReg = colour;
	GraphicsCommandReg = DrawHLine;
}

/*
 * draws a vertical line starting at pixel (x,y)
 * implemented in hardware
 */

void VLine(int x1, int y1, int length, int colour)
{
	WAIT_FOR_GRAPHICS;
	GraphicsX1Reg = x1;
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x1;
	GraphicsY2Reg = y1+length;
	GraphicsColourReg = colour;
	GraphicsCommandReg = DrawVLine;
}
/*
 * Draws an unfilled rectangle
 * (x1, y1) is the upper left corner of the rectangle
 * (x2, y2) is the bottom right corner
 */

void Rectangle(int x1, int y1, int x2, int y2, int colour)
{
	// draw top horizontal line
	WAIT_FOR_GRAPHICS;
	GraphicsX1Reg = x1;
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x2;
	GraphicsY2Reg = y1;
	GraphicsColourReg = colour;
	GraphicsCommandReg = DrawHLine;
	// draw bottom horizontal line
	WAIT_FOR_GRAPHICS;
	GraphicsX1Reg = x1;
	GraphicsY1Reg = y2;
	GraphicsX2Reg = x2;
	GraphicsY2Reg = y2;
	GraphicsColourReg = colour;
	GraphicsCommandReg = DrawHLine;
	// draw left vertical line
	WAIT_FOR_GRAPHICS;
	GraphicsX1Reg = x1;
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x1;
	GraphicsY2Reg = y2;
	GraphicsColourReg = colour;
	GraphicsCommandReg = DrawVLine;
	// draw right vertical line
	WAIT_FOR_GRAPHICS;
	GraphicsX1Reg = x2;
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x2;
	GraphicsY2Reg = y2;
	GraphicsColourReg = colour;
	GraphicsCommandReg = DrawVLine;

}


/*
 * Draws an equilateral triangle starting from
 * the bottom left at pixel coordinate (x1, y1)
 * @param length: size in pixels of each edge of the triangle 
 */
void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int colour)
{
    Line(x1, y1, x2, y2, colour);
    Line(x2, y2, x3, y3, colour);
    Line(x1, y1, x3, y3, colour);
}  
    
void DrawString(int x1, int y1, char* string, int strlen, int colour)
{
	y1 = y1-7;
	int i;
	for(i=0; i<strlen ; i++)
	{
		OutGraphicsCharFont2a(x1+i*FONT2_XPIXELS, y1, colour, WHITE, string[i], 0);
	}
}

/*
 * Draws a line from pixel coordinate (x1, y1) to (x2, y2)
 * implemented in hardware using bresenham's algorithm
 */

void Line(int x1, int y1, int x2, int y2, int colour)
{
	WAIT_FOR_GRAPHICS;
	GraphicsX1Reg = x1;
	GraphicsY1Reg = y1;
	GraphicsX2Reg = x2;
	GraphicsY2Reg = y2;
	GraphicsColourReg = colour;
	GraphicsCommandReg = Drawline;
}


void DrawButton(int x1, int y1, int x2, int y2, char* string, int strlen, int fontcolour, int backgroundcolour)
{
	/* center the string coordinats inside the button */
	int string_y = y1+(y2-y1)/2;
	int string_x = x1+(x2-x1)/2;
	string_x -= strlen/2 * FONT2_XPIXELS;

	FilledRectangle(x1, y1, x2, y2, backgroundcolour);
	DrawString(string_x, string_y, string, strlen, fontcolour);

}

void DrawMainMenu()
{
	FilledRectangle(0, 0, 800, 480, SILVER);
	DrawButton(50, 50, 375, 225, "Past Trips", sizeof("Past Trips")-1, BLACK, BEIGE);
	DrawButton(50, 255, 375, 430, "Easter Eggs", sizeof("Easter Eggs")-1, BLACK, BEIGE);
	DrawButton(400, 50, 750, 430, "New Trip", sizeof("New Trip")-1, BLACK, BEIGE);

}


void TriangleTest()
{
	int j = 0;
	while(1)
	{
		int i;
		for(i=0; i<400; i++)
		{
			if(j%2 == 0)
				Triangle(i, 1, 400, 480, 800-i, 0, i%256);
			else
				Triangle(i, 1, 400, 480, 800-i, 0, (256-i)%256);

		}
		for(i=0; i<400; i++)
		{
			if(j%2 == 0)
				Triangle(i, 480, 400, 0, 800-i, 480, i);
			else
				Triangle(i, 480, 400, 0, 800-i, 480, 256-i);

		}
		for(i=0; i<240; i++)
		{
			if(j%2 == 0)
				Triangle(0, i, 400, 480, 0, 240-i, i%256);
			else
				Triangle(0, i, 400, 480, 0, 2, (256-i)%256);

		}
		for(i=0; i<240; i++)
		{
			if(j%2 == 0)
				Triangle(i, 480, 400, 0, 800-i, 480, i);
			else
				Triangle(i, 480, 400, 0, 800-i, 480, 256-i);

		}
		j++;
	}
}














