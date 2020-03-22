/**
 * @file Raster.h
 * @discription Definition of the rasterization kernel.
 * @author Navi Ning <superokvv@outlook.com>
 */

#pragma once

#include "CELLMath.hpp"

namespace CELL {
	class Raster
	{
	public:
		Rgba* _buffer;
		int _width;
		int _height;
	public:
		Raster(int w, int h, void* buffer);
		~Raster();

		void clear();

		void drawPoint(int x, int y, Rgba color, int ptSize);

	private:
		inline void setPixel(unsigned x, unsigned y, Rgba color) {
			if (x >= _width || y >= _height) {
				return;
			}
			_buffer[y *_width + x] = color;
		}
	};

}