#include "Raster.h"

void draw(Raster &raster) {
	int2 pt[3] = {
		int2(230, -150),
		int2(10, 100),
		int2(460, 100),
	};

	Rgba color[3] = {
		Rgba(255, 0, 0),
		Rgba(0, 255, 0),
		Rgba(0, 0, 255)
	};
	for (int i = 0; i < 100; i++) {
		raster.drawTriangle(pt[0], pt[1], pt[2], color[0], color[1], color[2]);
	}
}
