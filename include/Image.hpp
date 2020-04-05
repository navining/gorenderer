#pragma once
#include "CELLMath.hpp"
#include "FreeImage.h"

namespace CELL {
	class Image {
	protected:
		int _width;
		int _height;
		Rgba *_pixel;
	public:
		Image(int w, int h, void *data) {
			if (w == 0 || h == 0 || data == 0) {
				_width = 0;
				_height = 0;
				_pixel = 0;
			}
			else {
				_width = w;
				_height = h;
				_pixel = new Rgba[w * h];
				memcpy(_pixel, data, w * h * sizeof(Rgba));
			}
		}

		~Image() {
			delete[] _pixel;
		}

		int width() const { return _width; }

		int height() const { return _height; }

		Rgba pixelAt(int x, int y) const {
			return _pixel[y * _width + x];
		}

	public:
		static Image* loadFromFile(const char*);
	};
}