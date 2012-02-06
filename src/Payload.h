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

// Payloads are what defines an encoding
// Changing the payload results in a different message being
//	sent and decoded.
// Offline payloads require that all the frames are kept until
//	the end.
// Online codecs (e.g. balanced which uses dynamic threshold
//	creation), do not need to work on all the frames at the end
//	all at once

namespace ofxGraycode {
	using namespace std;

	/** A generic payload class **/
	class Payload {
	public:
		Payload();
		void init(uint width, uint height);

		bool isAllocated() const;

		///Determines whether the payload
		///	needs the decoder to keep a
		///	copy of all the captures for
		///	the end
		virtual bool isOffline() const  {return false; }

		uint getWidth() const;
		uint getHeight() const;
		uint getSize() const;
		uint getFrameCount() const;
		virtual ofImageType getImageType() const = 0;

		bool matchingPixels(const ofPixels& pixels) const;
		void allocatePixels(ofPixels& pixels) const;
		virtual void fillPixels(const unsigned int frame, ofPixels& pixels) const = 0;
		virtual void readPixels(const unsigned int frame, const ofPixels& pixels) { };

	protected:
		virtual void render() = 0;

		bool allocated;
		uint width, height;
		uint size;
		uint frameCount;
	};

	class PayloadOffline : public Payload {
	public:
		bool isOffline() const { return true; }
		virtual void calc(const vector<ofPixels> &captures, const ofPixels& threshold, ofPixels_<uint> &output) const = 0;
	};

	class PayloadOnline : public Payload {
	public:
		bool isOffline() const { return false; }
		virtual void calc(ofPixels_<uint> &output) = 0;
	};

	/** Basic Graycode encoding with threshold taken from
		average value per pixel over all scan frames **/
	class PayloadGraycode : public PayloadOffline {
	public:
		ofImageType getImageType() const;
		void fillPixels(const unsigned int frame, ofPixels& pixels) const;
		void calc(const vector<ofPixels> &captures, const ofPixels& threshold, ofPixels_<uint> &output) const;

	protected:
		void render();

		uint getMaxIndex();

		uint frameCountX, frameCountY;

		ofPixels_<uint> data;
		vector<uint> dataInverse;
	};
}