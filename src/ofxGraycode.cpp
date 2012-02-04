//
//  ofxGraycode.cpp
//  ofxGraycode
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//
#include "ofxGraycode.h"

namespace ofxGraycode {
	Encoder::Encoder() {
		this->frame = 0;
		this->payload = 0;
	}

	void Encoder::init(Payload& payload) {
		this->payload = &payload;
		this->frame = 0;
	}

	void Encoder::reset() {
		frame = 0;
	}

	bool Encoder::operator>>(ofPixels& pixels) {

		if (frame >= payload->getFrameCount())
			return false;

		if (!payload->matchingPixels(pixels))
			payload->allocatePixels(pixels);

		payload->fillPixels(frame++, pixels);

		return true;
	}

	bool Encoder::operator>>(ofImage& image) {
		if (operator>>(image.getPixelsRef())) {
			image.update();
			return true;
		} else
			return false;
	}

}