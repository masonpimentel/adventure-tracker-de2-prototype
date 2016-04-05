/*
 * Fill.h
 *
 *  Created on: Feb 2, 2016
 *      Author: mitchell
 */

#ifndef FILL_H_
#define FILL_H_
#include "graphics.h"

typedef struct { int x,y, poi;} XYPixel ;

void Fill(int _x, int _y, int _FillColour, int _BoundaryColour);
void FilledRectangle(int x1,int y1, int x2, int y2, int colour);

#endif /* FILL_H_ */
