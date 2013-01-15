//
//  Payload.h
//  ofxGraycode
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//
#pragma once
#include "ofMain.h"

#include "DataSet.h"

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
		void init(uint32_t width, uint32_t height);

		bool isAllocated() const;

		///Determines whether the payload
		///	needs the decoder to keep a
		///	copy of all the captures for
		///	the end
		virtual bool isOffline() const  {return false; }

		uint32_t getWidth() const;
		uint32_t getHeight() const;
		uint32_t getSize() const;
		uint32_t getFrameCount() const;
		virtual ofImageType getImageType() const = 0;

		bool matchingPixels(const ofPixels& pixels) const;
		void allocatePixels(ofPixels& pixels) const;
		virtual void fillPixels(const unsigned int frame, ofPixels& pixels) const = 0;
		virtual void readPixels(const unsigned int frame, const ofPixels& pixels) const { };

	protected:
		virtual void render() = 0;

		bool allocated;
		uint32_t width, height;
		uint32_t size;
		uint32_t frameCount;
	};

	class PayloadOffline : public Payload {
	public:
		bool isOffline() const { return true; }
		virtual void calc(const vector<ofPixels> &captures, DataSet &data) const = 0;
	};

	class PayloadOnline : public Payload {
	public:
		bool isOffline() const { return false; }
		virtual void calc(DataSet& data) const = 0;
	};

	/** Basic Graycode encoding with threshold taken from
		average value per pixel over all scan frames **/
	class PayloadGraycode : public PayloadOffline {
	public:
		ofImageType getImageType() const;
		void fillPixels(const unsigned int frame, ofPixels& pixels) const;
		void calc(const vector<ofPixels> &captures, DataSet &data) const;
		
	protected:
		void render();

		uint32_t getMaxIndex();

		uint32_t frameCountX, frameCountY;

		ofPixels_<uint32_t> data;
		vector<uint32_t> dataInverse;
	};
}