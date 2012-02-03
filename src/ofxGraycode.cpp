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

	bool Encoder::operator>>(ofBaseHasPixels& image) {

		if (frame == payload->getFrameCount())
			return false;

		ofPixels& output = image.getPixelsRef();

		if (!payload->matchingPixels(output))
			payload->allocatePixels(output);

		payload->fillPixels(frame, output);

		return true;
	}

}