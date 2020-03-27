/**
 * @file Raster.h
 * @discription Definition of the rasterization kernel.
 * @author Navi Ning <superokvv@outlook.com>
 */

#pragma once

#include "CELLMath.hpp"
using namespace CELL;

enum DRAWMODE {
	DM_POINTS = 0,
	DM_LINES = 1,
	DM_LINE_LOOP = 2,
	DM_LINE_STRIP = 3
};

class Span {
public:
	int _xStart;
	int _xEnd;
	Rgba _colorStart;
	Rgba _colorEnd;
	int _y;
public:
	Span(int xStart, int xEnd, int y, Rgba colorStart, Rgba colorEnd);
};

class Edge {
public:
	int _x1;
	int _y1;
	Rgba _color1;
	int _x2;
	int _y2;
	Rgba _color2;
public:
	Edge(int x1, int y1, Rgba color1, int x2, int y2, Rgba color2);
};

class Raster
{
public:
	Rgba* _buffer;
	int _width;
	int _height;
	Rgba _color;
public:
	Raster(int w, int h, void* buffer);
	~Raster();

	// Clear the buffer
	void clear();

	// Draw a point
	void drawPoint(int x, int y, Rgba color, int ptSize);

	// Draw according to DRAWMODE
	void drawArrays(DRAWMODE mode, const float2* points, int count);

	// Draw a filled rectangle
	void drawFilledRect(int startX, int startY, int w, int h);

	// Draw a rectangle
	void drawRect(const int2* points, const Rgba* colors);

	// Draw a line
	void drawLine(float2 pt1, float2 pt2, Rgba color1, Rgba color2);

	// Draw a triangle
	void drawTriangle(int2 p0, int2 p1, int2 p2, Rgba c0, Rgba c1, Rgba c2);

private:
	// Set a color for a pixel
	void setPixel(unsigned x, unsigned y, Rgba color);;

	// Set a color for a pixel (without boundary check)
	void setPixelEx(unsigned x, unsigned y, Rgba color);;

	// Draw a point
	void drawPoint(float2 pt, Rgba color);

	// Draw a horizonal span
	void drawSpan(const Span& span);
	
	// Draw a triangle with two edges (e1.y > e2.y)
	void drawEdge(const Edge& e1, const Edge& e2);

	// Check if the point locates inside the viewport
	bool isInRect(int2 pt);
};
