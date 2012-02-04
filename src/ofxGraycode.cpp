//
//  ofxGraycode.cpp
//  ofxGraycode
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//
#include "ofxGraycode.h"

namespace ofxGraycode {
	
//----------------------------------------
// BaseCodec
	BaseCodec::BaseCodec() {
		this->frame = 0;
		this->payload = 0;
	}

	void BaseCodec::init(Payload& payload) {
		this->payload = &payload;
		reset();
	}

//----------------------------------------
// Encoder
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
	
//----------------------------------------
// Decoder
	void Decoder::reset() {
		frame = 0;
	}

	void Decoder::operator<<(ofPixels& pixels) {
		if (frame==0)
			data.allocate(pixels.getWidth(), pixels.getHeight(), OF_IMAGE_GRAYSCALE);

		if (this->payload->isOffline())
			captures.push_back(ofPixels(pixels));
		else
			payload->readPixels(frame, pixels);

 		if (++frame >= payload->getFrameCount()) {
			calc();
			frame = 0;
		}
	}

	void Decoder::operator<<(ofBaseHasPixels& image) {
		this->operator<<(image.getPixelsRef());
	}

	bool Decoder::isReady() {
		return data.isAllocated();
	}

	////
	//ofBaseDraws
	////
	//
	void Decoder::draw(float x,float y) {
		preview.draw(x, y);
	}

	void Decoder::draw(float x,float y,float w, float h) {
		preview.draw(x, y, w, h);
	}
	
	float Decoder::getHeight() {
		return preview.getHeight();
	}

	float Decoder::getWidth() {
		return preview.getWidth();
	}
	//
	////

	void Decoder::calc() {
		if (payload->isOffline()) {
			PayloadOffline& payload(*(PayloadOffline*)this->payload);
			payload.calc(this->captures, this->data);
		} else {
			PayloadOnline& payload(*(PayloadOnline*)this->payload);
			payload.calc(this->data);
		}
		updatePreview();
	}

	void Decoder::updatePreview() {
		preview.allocate(data.getWidth(), data.getHeight(), OF_IMAGE_COLOR);
		uchar* pix = preview.getPixels();
		const uint* idx = data.getPixels();
		for (int i=0; i<data.size(); i++, idx++) {
			if (*idx < payload->getSize()) {
				*pix++ = 255.0f * float(*idx % payload->getWidth()) / float(payload->getWidth());
				*pix++ = 255.0f * float(*idx / payload->getHeight()) / float(payload->getHeight());
				*pix++;
			} else
				pix+=3;
		}
		preview.update();
	}
}