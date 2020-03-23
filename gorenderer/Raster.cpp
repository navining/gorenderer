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

	void Raster::drawLine(float2 pt1, float2 pt2, Rgba color) {
		float xOffset = pt1.x - pt2.x;
		float yOffset = pt1.y - pt2.y;

		if (xOffset == 0 && yOffset == 0) {
			setPixel(pt1.x, pt1.y, color);
		}

		if (xOffset == 0) {
			float yMin, yMax;
			if (pt1.y < pt2.y) {
				yMin = pt1.y;
				yMax = pt2.y;
			}
			else {
				yMin = pt2.y;
				yMax = pt1.y;
			}
			for (float y = yMin; y <= yMax; y += 1.0f) {
				setPixel(pt1.x, y, color);
			}
		}
		else if (yOffset == 0) {
			float xMin, xMax;
			if (pt1.x < pt2.x) {
				xMin = pt1.x;
				xMax = pt2.x;
			}
			else {
				xMin = pt2.x;
				xMax = pt1.x;
			}
			for (float x = xMin; x <= xMax; x += 1.0f) {
				setPixel(x, pt1.y, color);
			}
		}
		else {
			if (fabs(xOffset) > fabs(yOffset)) {
				float xMin, xMax;
				if (pt1.x < pt2.x) {
					xMin = pt1.x;
					xMax = pt2.x;
				}
				else {
					xMin = pt2.x;
					xMax = pt1.x;
				}
				float slope = yOffset / xOffset;
				for (float x = xMin; x <= xMax; x += 1.0f) {
					float y = pt1.y + slope * (x - pt1.x);
					setPixel(x, y, color);
				}
			}
			else {
				float yMin, yMax;
				if (pt1.y < pt2.y) {
					yMin = pt1.y;
					yMax = pt2.y;
				}
				else {
					yMin = pt2.y;
					yMax = pt1.y;
				}
				float slope = xOffset / yOffset;
				for (float y = yMin; y <= yMax; y += 1.0f) {
					float x = pt1.x + slope * (y - pt1.y);
					setPixel(x, y, color);
				}
			}

		}
	}

}