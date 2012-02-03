//
//  Payload.cpp
//  ofxGraycode
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//
#include "Payload.h"

namespace ofxGraycode {
	
	Payload::Payload() {
		this->allocated = false;
	}

	void Payload::init(unsigned long width, unsigned long height) {
		this->width = width;
		this->height = height;
		this->size = width*height;
		render();
		this->allocated = true;
	}

	bool Payload::isAllocated() const {
		return this->allocated;
	}

	ulong Payload::getWidth() const {
		return this->width;
	}

	ulong Payload::getHeight() const {
		return this->height;
	}

	ulong Payload::getFrameCount() const {
		return this->frameCount;
	}

	bool Payload::matchingPixels(const ofPixels& pixels) const {
		return pixels.getWidth() == this->getWidth() &&
			pixels.getHeight() == this->getHeight() &&
			pixels.getImageType() == this->getImageType();
	}

	void Payload::allocatePixels(ofPixels& pixels) const {
		pixels.allocate(this->getWidth(), this->getHeight(), this->getImageType());
	}

	ofImageType PayloadGraycode::getImageType() const {
		return OF_IMAGE_GRAYSCALE;
	}

	void PayloadGraycode::encode(const unsigned int frame, ofPixels& pixels) const {
		if (!isAllocated()) {
			ofLogError() << "ofxGraycode::PayloadGraycode::encode : cannot encode, we are not allocated";
			return;
		}
	} 

	void PayloadGraycode::render() {
		this->frameCountX = ceil(log((float)width) / log((float)2)); 
		this->frameCountY = ceil(log((float)width) / log((float)2)); 
		this->frameCount = frameCountX + frameCountY;
		
		data.allocate(width, height, OF_IMAGE_GRAYSCALE);
		dataInverse.allocate(width, height, OF_IMAGE_GRAYSCALE);
		
		ulong* pix = data.getPixels();
		ulong idx;

		for (ulong y=0; y<height; y++)
			for (ulong x=0; x<width; x++, pix++, idx++) {
				*pix = x ^ (x >> 1) +
					((y ^ (y >> 1)) << (int) frameCountX);
				dataInverse[*pix] = idx;
			}
	}

	ulong PayloadGraycode::getMaxIndex() {
		return 1 << (frameCountX + frameCountY);
	}
}