/**
 * @file Raster.cpp
 * @discription Implementation of the rasterization kernel.
 * @author Navi Ning <superokvv@outlook.com>
 */

#include "Raster.h"
using namespace CELL;

Span::Span(int xStart, int xEnd, int y, Rgba colorStart, Rgba colorEnd) {
	if (xStart < xEnd) {
		_xStart = xStart;
		_xEnd = xEnd;
		_colorEnd = colorEnd;
		_colorStart = colorStart;
		_y = y;
	}
	else {
		_xStart = xEnd;
		_xEnd = xStart;
		_colorEnd = colorStart;
		_colorStart = colorEnd;
		_y = y;
	}
}

Edge::Edge(int x1, int y1, Rgba color1, int x2, int y2, Rgba color2) {
	if (y1 < y2) {
		_x1 = x1;
		_y1 = y1;
		_color1 = color1;

		_x2 = x2;
		_y2 = y2;
		_color2 = color2;
	}
	else {
		_x1 = x2;
		_y1 = y2;
		_color1 = color2;

		_x2 = x1;
		_y2 = y1;
		_color2 = color1;
	}
}

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

void Raster::drawFilledRect(int startX, int startY, int w, int h) {
	int left = tmax<int>(startX, 0);
	int top = tmax<int>(startY, 0);
	int right = tmin<int>(startX + w, _width);
	int bottom = tmin<int>(startY + h, _height);
	for (int x = left; x < right; x++) {
		for (int y = top; y < bottom; y++) {
			setPixelEx(x, y, _color);
		}
	}
}

void Raster::drawRect(const int2 * points, const Rgba * colors) {
	int left = tmax<int>(points[0].x, 0);
	int top = tmax<int>(points[0].y, 0);
	int right = tmin<int>(points[2].x, _width);
	int bottom = tmin<int>(points[2].y, _height);
	float w = right - left;
	float h = bottom - top;
	for (int x = left; x < right; x++) {
		Rgba color1 = colorLerp(colors[0], colors[1], (x - left) / w);
		Rgba color2 = colorLerp(colors[3], colors[2], (x - left) / w);
		for (int y = top; y < bottom; y++) {
			Rgba color = colorLerp(color1, color2, (y - top) / h);
			setPixelEx(x, y, color);
		}
	}
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
		Rgba colorMin, colorMax;
		if (pt1.x < pt2.x) {
			xMin = pt1.x;
			xMax = pt2.x;
			colorMin = color1;
			colorMax = color2;
		}
		else {
			xMin = pt2.x;
			xMax = pt1.x;
			colorMin = color2;
			colorMax = color1;
		}
		float length = xMax - xMin;
		for (float x = xMin; x <= xMax; x += 1.0f) {
			float y = pt1.y + slope * (x - pt1.x);
			float scaler = (x - xMin) / length;
			Rgba color = colorLerp(colorMin, colorMax, scaler);
			setPixel(x, y, color);
		}
	}
	else {
		float slope = xOffset / yOffset;
		float yMin, yMax;
		Rgba colorMin, colorMax;
		if (pt1.y < pt2.y) {
			yMin = pt1.y;
			yMax = pt2.y;
			colorMin = color1;
			colorMax = color2;
		}
		else {
			yMin = pt2.y;
			yMax = pt1.y;
			colorMin = color2;
			colorMax = color1;
		}

		float length = yMax - yMin;
		for (float y = yMin; y <= yMax; y += 1.0f) {
			float x = pt1.x + slope * (y - pt1.y);
			float scaler = (y - yMin) / length;
			Rgba color = colorLerp(colorMin, colorMax, scaler);
			setPixel(x, y, color);
		}
	}

}

inline Rgba CELL::Raster::getPixel(unsigned x, unsigned y)
{
	return _buffer[y *_width + x];
}

inline void Raster::setPixel(unsigned x, unsigned y, Rgba color) {
	if (x >= _width || y >= _height) {
		return;
	}
	_buffer[y *_width + x] = color;
}

inline void Raster::setPixelEx(unsigned x, unsigned y, Rgba color) {
	_buffer[y *_width + x] = color;
}


inline void Raster::drawPoint(float2 pt, Rgba color) {
	setPixel(pt.x, pt.y, color);
}

void Raster::drawSpan(const Span & span) {
	float length = span._xEnd - span._xStart;
	float scale = 0;
	float step = 1.0f / length;

	// Viewport clip
	int startX = tmax<int>(span._xStart, 0);
	int endX = tmin<int>(span._xEnd, _width);
	scale += (startX - span._xStart) / length;

	for (int x = startX; x <= endX; x++) {
		Rgba color = colorLerp(span._colorEnd, span._colorStart, scale);
		setPixelEx(x, span._y, color);
		scale += step;
	}
}


inline void Raster::drawEdge(const Edge & e1, const Edge & e2) {
	if (e1._y2 == e1._y1 || e2._y2 == e2._y1) return;

	float xOffset1 = e1._x2 - e1._x1;
	float yOffset1 = e1._y2 - e1._y1;
	float scale1 = (e2._y1 - e1._y1) / yOffset1;
	float step1 = 1 / yOffset1;

	float xOffset2 = e2._x2 - e2._x1;
	float yOffset2 = e2._y2 - e2._y1;
	float scale2 = 0;
	float step2 = 1 / yOffset2;

	// Viewport clip
	int startY1 = tmax<int>(e1._y1, 0);
	int endY1 = tmin<int>(e1._y2, _height);
	scale1 += (startY1 - e1._y1) / yOffset1;

	int startY2 = tmax<int>(e2._y1, 0);
	int endY2 = tmin<int>(e2._y2, _height);
	scale2 += (startY2 - e2._y1) / yOffset2;

	for (int y = startY2; y <= endY2; y++) {
		int x1 = e1._x1 + scale1 * xOffset1;
		int x2 = e2._x1 + scale2 * xOffset2;

		Rgba color2 = colorLerp(e2._color1, e2._color2, scale2);
		Rgba color1 = colorLerp(e1._color1, e1._color2, scale1);

		Span span(x1, x2, y, color1, color2);
		drawSpan(span);

		scale1 += step1;
		scale2 += step2;
	}
}


inline bool Raster::isInRect(int2 pt) {
	if (pt.x >= 0 && pt.x <= _width && pt.y >= 0 && pt.y <= _height) {
		return true;
	}
	return false;
}

void Raster::drawTriangle(int2 p0, int2 p1, int2 p2, Rgba c0, Rgba c1, Rgba c2) {

	if (!isInRect(p0) && !isInRect(p1) && !isInRect(p2)) {
		return;
	}

	Edge edges[] = {
		Edge(p0.x, p0.y, c0, p1.x, p1.y, c1),
		Edge(p1.x, p1.y, c1, p2.x, p2.y, c2),
		Edge(p2.x, p2.y, c2, p0.x, p0.y, c0)
	};

	// Find the longest edge
	int iMax = 0;
	int length = edges[0]._y2 - edges[0]._y1;
	for (int i = 0; i < 3; i++) {
		int len = edges[i]._y2 - edges[i]._y1;
		if (len > length) {
			length = len;
			iMax = i;
		}
	}

	int iMin1 = (iMax + 1) % 3;
	int iMin2 = (iMax + 2) % 3;
	drawEdge(edges[iMax], edges[iMin1]);
	drawEdge(edges[iMax], edges[iMin2]);
}

void Raster::drawImage(int startX, int startY, const Image * image) {
	if (image == NULL) return;
	int left = tmax<int>(startX, 0);
	int top = tmax<int>(startY, 0);
	int right = tmin<int>(startX + image->width(), _width);
	int bottom = tmin<int>(startY + image->height(), _height);
	for (int x = left; x < right; x++) {
		for (int y = top; y < bottom; y++) {
			Rgba color = image->pixelAt(x - left, y - top);
			setPixelEx(x, y, color);
		}
	}
}

void CELL::Raster::drawImage(int startX, int startY, const Image * image, int x1, int y1, int w, int h) {
	if (image == NULL) return;
	int left = tmax<int>(startX, 0);
	int top = tmax<int>(startY, 0);
	int right = tmin<int>(startX + w, _width);
	int bottom = tmin<int>(startY + h, _height);
	for (int x = left; x < right; x++) {
		for (int y = top; y < bottom; y++) {
			Rgba color = image->pixelAt(x - left + x1, y - top + y1);
			setPixelEx(x, y, color);
		}
	}
}

void Raster::drawImageColorKey(int startX, int startY, const Image * image, Rgba key) {
	if (image == NULL) return;
	int left = tmax<int>(startX, 0);
	int top = tmax<int>(startY, 0);
	int right = tmin<int>(startX + image->width(), _width);
	int bottom = tmin<int>(startY + image->height(), _height);
	for (int x = left; x < right; x++) {
		for (int y = top; y < bottom; y++) {
			Rgba color = image->pixelAt(x - left, y - top);
			if (color != key)
				setPixelEx(x, y, color);
		}
	}
}

void Raster::drawImageAlphaTest(int startX, int startY, const Image * image, byte alpha) {
	if (image == NULL) return;
	int left = tmax<int>(startX, 0);
	int top = tmax<int>(startY, 0);
	int right = tmin<int>(startX + image->width(), _width);
	int bottom = tmin<int>(startY + image->height(), _height);
	for (int x = left; x < right; x++) {
		for (int y = top; y < bottom; y++) {
			Rgba color = image->pixelAt(x - left, y - top);
			if (color._a > alpha)
				setPixelEx(x, y, color);
		}
	}
}

void Raster::drawImageAlphaBlend(int startX, int startY, const Image * image, float alpha) {
	if (image == NULL) return;
	int left = tmax<int>(startX, 0);
	int top = tmax<int>(startY, 0);
	int right = tmin<int>(startX + image->width(), _width);
	int bottom = tmin<int>(startY + image->height(), _height);
	for (int x = left; x < right; x++) {
		for (int y = top; y < bottom; y++) {
			Rgba srcColor = image->pixelAt(x - left, y - top);
			Rgba dstColor = getPixel(x, y);
			Rgba color = colorLerp(dstColor, srcColor, srcColor._a / 255.0f * alpha);
			setPixelEx(x, y, color);
		}
	}
}

void CELL::Raster::drawImageAlpha(int startX, int startY, const Image * image, float alpha) {
	if (image == NULL) return;
	int left = tmax<int>(startX, 0);
	int top = tmax<int>(startY, 0);
	int right = tmin<int>(startX + image->width(), _width);
	int bottom = tmin<int>(startY + image->height(), _height);
	for (int x = left; x < right; x++) {
		for (int y = top; y < bottom; y++) {
			Rgba srcColor = image->pixelAt(x - left, y - top);
			Rgba dstColor = getPixel(x, y);
			Rgba color = colorLerp(dstColor, srcColor, alpha);
			setPixelEx(x, y, color);
		}
	}
}

void CELL::Raster::drawImageScale(int dstX, int dstY, int dstW, int dstH, const Image * image) {
	float xScale = image->width() / (float)dstW;
	float yScale = image->height() / (float)dstH;

	for (int x = dstX; x < dstX + dstW; x++) {
		for (int y = dstY; y < dstY + dstH; y++) {
			float xx = (x - dstX) * xScale;
			float yy = (y - dstY) * yScale;

			Rgba color = image->pixelAt(xx, yy);
			setPixelEx(x, y, color);
		}
	}
}

void CELL::Raster::drawImageScaleHighQuality(int dstX, int dstY, int dstW, int dstH, const Image * image) {
	float xScale = image->width() / (float)dstW;
	float yScale = image->height() / (float)dstH;

	for (int x = dstX; x < dstX + dstW; x++) {
		for (int y = dstY; y < dstY + dstH; y++) {
			float xx = (x - dstX) * xScale;
			float yy = (y - dstY) * yScale;
			Rgba color;

			
			if (xx >= image->width() - 1 && yy >= image->height() - 1){
				color = image->pixelAt(xx, yy);
			} else if (xx >= image->width() - 1) {
				Rgba colorY1 = image->pixelAt(xx, yy);
				Rgba colorY2 = image->pixelAt(xx, yy + 1);
				color = colorLerp(colorY1, colorY2, yy - (int)yy);
			} else if (yy >= image->height() - 1){
				Rgba colorX1 = image->pixelAt(xx, yy);
				Rgba colorX2 = image->pixelAt(xx + 1, yy);
				color = colorLerp(colorX1, colorX2, xx - (int)xx);
			} else {
				Rgba colorX1Y1 = image->pixelAt(xx, yy);
				Rgba colorX2Y1 = image->pixelAt(xx + 1, yy);
				Rgba colorX1Y2 = image->pixelAt(xx, yy + 1);
				Rgba colorX2Y2 = image->pixelAt(xx + 1, yy + 1);
				Rgba colorY1 = colorLerp(colorX1Y1, colorX2Y1, xx - (int)xx);
				Rgba colorY2 = colorLerp(colorX1Y2, colorX2Y2, xx - (int)xx);
				color = colorLerp(colorY1, colorY2, yy - (int)yy);
			}
			setPixelEx(x, y, color);
		}
	}
}

Image* Image::loadFromFile(const char* fileName) {
	// Get image format
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
	if (fifmt == FIF_UNKNOWN) {
		return NULL;
	}

	// Load image
	FIBITMAP *dib = FreeImage_Load(fifmt, fileName, 0);
	FREE_IMAGE_COLOR_TYPE type = FreeImage_GetColorType(dib);

	// Get data pointer
	FIBITMAP *temp = dib;
	dib = FreeImage_ConvertTo32Bits(dib);
	FreeImage_Unload(temp);

	BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
	int width = FreeImage_GetWidth(dib);
	int height = FreeImage_GetHeight(dib);

	Image *image = new Image(width, height, pixels);

	FreeImage_Unload(dib);

	return image;
}