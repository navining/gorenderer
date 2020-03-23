/**
 * @file Raster.h
 * @discription Definition of the rasterization kernel.
 * @author Navi Ning <superokvv@outlook.com>
 */

#pragma once

#include "CELLMath.hpp"
namespace CELL {
	enum DRAWMODE {
		DM_POINTS = 0,
		DM_LINES = 1,
		DM_LINE_LOOP = 2,
		DM_LINE_STRIP = 3
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

		void clear();

		void drawPoint(int x, int y, Rgba color, int ptSize);

		void drawArrays(DRAWMODE mode, const float2* points, int count);

	private:
		inline void setPixel(unsigned x, unsigned y, Rgba color);

		void drawPoint(float2 pt, Rgba color) {
			setPixel(pt.x, pt.y, color);
		}

		void drawLine(float2 pt1, float2 pt2, Rgba color1, Rgba color2);
	};

}