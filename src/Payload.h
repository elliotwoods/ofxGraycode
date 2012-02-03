//
//  Payload.h
//  ofxGraycode
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//
#pragma once
#include "ofMain.h"

#include "Types.h"

namespace ofxGraycode {
	using namespace std;

	/** A generic payload class **/
	class Payload {
	public:
		Payload();
		void init(ulong width, ulong height);

		bool isAllocated() const;

		ulong getWidth() const;
		ulong getHeight() const;
		ulong getFrameCount() const;
		virtual ofImageType getImageType() const = 0;

		bool matchingPixels(const ofPixels& pixels) const;
		void allocatePixels(ofPixels& pixels) const;
		virtual void fillPixels(const unsigned int frame, ofPixels& pixels) const = 0;


	protected:
		virtual void render() = 0;

		bool allocated;
		unsigned long width, height;
		unsigned long size;
		unsigned long frameCount;
	};

	class PayloadGraycode : public Payload {
	public:
		ofImageType getImageType() const;
		void encode(const unsigned int frame, ofPixels& pixels) const;
		
	protected:
		void render();

		ulong getMaxIndex();

		ulong frameCountX, frameCountY;

		ofPixels_<ulong> data;
		ofPixels_<ulong> dataInverse;
	};
}