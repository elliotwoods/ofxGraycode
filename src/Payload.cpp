//
//  Payload.cpp
//  ofxGraycode
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//
#include "Payload.h"

namespace ofxGraycode {
	
//----------------------------------------------------------
// Payload
	Payload::Payload() {
		this->allocated = false;
	}

	void Payload::init(uint width, uint height) {
		this->width = width;
		this->height = height;
		this->size = width*height;
		render();
		this->allocated = true;
	}

	bool Payload::isAllocated() const {
		return this->allocated;
	}

	uint Payload::getWidth() const {
		return this->width;
	}

	uint Payload::getHeight() const {
		return this->height;
	}
	
	uint Payload::getSize() const {
		return this->size;
	}

	uint Payload::getFrameCount() const {
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

//----------------------------------------------------------
// PayloadGraycode

	ofImageType PayloadGraycode::getImageType() const {
		return OF_IMAGE_GRAYSCALE;
	}

	void PayloadGraycode::fillPixels(const unsigned int frame, ofPixels& pixels) const {
		if (!isAllocated()) {
			ofLogError() << "ofxGraycode::PayloadGraycode::fillPixels : cannot encode, we are not allocated";
			return;
		}

		uchar* pixel = pixels.getPixels();
		const uint* data = this->data.getPixels();
		for (int i=0; i<size; i++)
			*pixel++ = (*data++ & (uint)1 << frame) == (uint)1 << frame ? (uchar)255 : (uchar)0;
	}

	void PayloadGraycode::calc(const vector<ofPixels> &captures, ofPixels_<uint> &output) {
		if (captures.size() != frameCount)
		{
			ofLogError() << "ofxGraycode::PayloadGraycode::calc : cannot calc as number of captured frames does not match target framecount";
			return;
		}

		//calculate the mean of all captures
		ofPixels mean = ofPixels(captures[0]);
		mean.set(0, 0);
		const uchar* pixelIn;
		uchar* pixelOut;
		for (uint frame=0; frame<frameCount; frame++) {
			pixelIn = captures[frame].getPixels();
			pixelOut = mean.getPixels();
			for (int i=0; i<mean.size(); i++) {
				*pixelOut++ += *pixelIn++ / frameCount;
			}
		}
		
		//prepare output
		output.allocate(captures[0].getWidth(), captures[0].getHeight(), OF_IMAGE_GRAYSCALE);
		output.set(0, 0);

		//decode
		const uchar* meanIn;
		uint* dataOut;
		for (uint frame=0; frame<frameCount; frame++) {
			pixelIn = captures[frame].getPixels();
			meanIn = mean.getPixels();
			dataOut = output.getPixels();
			for (int i=0; i<mean.size(); i++, dataOut++) {
				if (*pixelIn++ > *meanIn++)
					*dataOut |= (uint)1 << frame;
				else
					*dataOut;
			}
		}

		//invert data
		dataOut = output.getPixels();
		for (int i=0; i<data.size(); i++, dataOut++) {
			*dataOut = dataInverse[*dataOut];
		}
	}

	void PayloadGraycode::render() {
		this->frameCountX = ceil(log((float)width) / log((float)2)); 
		this->frameCountY = ceil(log((float)width) / log((float)2)); 
		this->frameCount = frameCountX + frameCountY;
		
		data.allocate(width, height, OF_IMAGE_GRAYSCALE);
		dataInverse.resize(getMaxIndex());
		
		uint* pix = data.getPixels();
		uint idx = 0;

		for (uint y=0; y<height; y++)
			for (uint x=0; x<width; x++, pix++, idx++) {
				*pix = x ^ (x >> 1) +
					((y ^ (y >> 1)) << (int) frameCountX);
				dataInverse[*pix] = idx;
			}
	}

	uint PayloadGraycode::getMaxIndex() {
		return 1 << (frameCountX + frameCountY);
	}
}