/**
 * @file Raster.cpp
 * @discription Implementation of the rasterization kernel.
 * @author Navi Ning <superokvv@outlook.com>
 */

#include "Raster.h"

namespace CELL {
	Raster::Raster(int w, int h, void* buffer)
	{
		_width = w;
		_height = h;
		_buffer = (Rgba*)buffer;
	}


	Raster::~Raster()
	{
	}

	void Raster::clear() {
		memset(_buffer, 0, _width * _height * sizeof(Rgba));
	}

	void Raster::drawPoint(int x, int y, Rgba color, int ptSize) {
		switch (ptSize) {
		case 1:
			setPixel(x, y, color);
			break;
		case 2:
			setPixel(x, y, color);
			setPixel(x + 1, y, color);
			setPixel(x, y + 1, color);
			setPixel(x + 1, y + 1, color);
			break;
		case 3:
			setPixel(x, y, color);
			setPixel(x - 1, y, color);
			setPixel(x + 1, y, color);
			setPixel(x, y - 1, color);
			setPixel(x, y + 1, color);
			setPixel(x - 1, y - 1, color);
			setPixel(x - 1, y + 1, color);
			setPixel(x + 1, y - 1, color);
			setPixel(x + 1, y + 1, color);
		}
	}

}