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

	void Raster::drawArrays(DRAWMODE mode, const float2 * points, int count) {
		switch (mode) {
		case DM_POINTS:
		{
			for (int i = 0; i < count; i++) {
				drawPoint(points[i], _color);
			}
		}
		break;
		case DM_LINES:
		{
			count = count / 2 * 2;
			for (int i = 0; i < count; i++) {
				drawLine(points[i], points[i + 1], _color, _color);
			}
		}
		break;
		case DM_LINE_LOOP:
			drawLine(points[0], points[1], _color, _color);
			for (int i = 2; i < count; i++) {
				drawLine(points[i - 1], points[i], _color, _color);
			}
			drawLine(points[0], points[count - 1], _color, _color);
			break;
		case DM_LINE_STRIP:
			drawLine(points[0], points[1], _color, _color);
			for (int i = 2; i < count; i++) {
				drawLine(points[i - 1], points[i], _color, _color);
			}
			break;
		default:
			break;
		}
	}

	inline void Raster::setPixel(unsigned x, unsigned y, Rgba color) {
		if (x >= _width || y >= _height) {
			return;
		}
		_buffer[y *_width + x] = color;
	}

	void Raster::drawLine(float2 pt1, float2 pt2, Rgba color1, Rgba color2) {
		float xOffset = pt1.x - pt2.x;
		float yOffset = pt1.y - pt2.y;

		if (xOffset == 0 && yOffset == 0) {
			setPixel(pt1.x, pt1.y, color1);
		}

		if (fabs(xOffset) > fabs(yOffset)) {
			float slope = yOffset / xOffset;
			float xMin, xMax;
			if (pt1.x < pt2.x) {
				xMin = pt1.x;
				xMax = pt2.x;
			}
			else {
				xMin = pt2.x;
				xMax = pt1.x;
			}
			float length = xMax - xMin;
			for (float x = xMin; x <= xMax; x += 1.0f) {
				float y = pt1.y + slope * (x - pt1.x);
				float scaler = (x - xMin) / length;
				Rgba color = colorLerp(color1, color2, scaler);
				setPixel(x, y, color);
			}
		}
		else {
			float slope = xOffset / yOffset;
			float yMin, yMax;
			if (pt1.y < pt2.y) {
				yMin = pt1.y;
				yMax = pt2.y;
			}
			else {
				yMin = pt2.y;
				yMax = pt1.y;
			}

			float length = yMax - yMin;
			for (float y = yMin; y <= yMax; y += 1.0f) {
				float x = pt1.x + slope * (y - pt1.y);
				float scaler = (y - yMin) / length;
				Rgba color = colorLerp(color1, color2, scaler);
				setPixel(x, y, color);
			}
		}

	}
}
