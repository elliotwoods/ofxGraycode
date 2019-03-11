#include "Encoder.h"

namespace ofxGraycode {
	//----------
	void Encoder::reset() {
		frame = 0;
	}

	//----------
	bool Encoder::operator>> (ofPixels& pixels) {
		if (frame >= payload->getFrameCount())
			return false;

		if (!payload->matchingPixels(pixels))
			payload->allocatePixels(pixels);

		payload->fillPixels(frame++, pixels);

		return true;
	}

	//----------
	bool Encoder::operator>> (ofImage& image) {
		if (operator >> (image.getPixels())) {
			image.update();
			return true;
		}
		else
			return false;
	}
}