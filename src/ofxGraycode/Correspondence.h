#pragma once
#include <stdint.h>

namespace ofxGraycode {
	class Correspondence{
	public:
		Correspondence(uint32_t cameraPixel, uint32_t projectorPixel) {
			this->cameraPixel = cameraPixel;
			this->projectorPixel = projectorPixel;
		}
		uint32_t cameraPixel;
		uint32_t projectorPixel;
	};
}