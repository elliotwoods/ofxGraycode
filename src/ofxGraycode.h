//
//  ofxGraycode.h
//  ofxGraycode
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//
#pragma once
#include "ofMain.h"

#include "Payload.h"

namespace ofxGraycode {
	using namespace std;

	class BaseCodec {
	public:
		BaseCodec();
		void init(Payload& payload);
		virtual void reset() = 0;
		int getFrame();
	protected:
		Payload* payload;
		int	frame;
	};


	class Encoder : public BaseCodec {
	public:
		void reset();

		bool operator>>(ofPixels& pixels);
		bool operator>>(ofImage& image);
	};


	class Decoder : public BaseCodec, ofBaseDraws {
	public:
		void reset();
		void operator<<(const ofPixels& pixels);
		void operator<<(ofBaseHasPixels& image);
		bool isReady();

		const vector<ofPixels>& getCaptures() const;
		const ofPixels& getMean() const;
		////
		//ofBaseDraws
		////
		//
		void draw(float x,float y);
		void draw(float x,float y,float w, float h);
		float getHeight();
		float getWidth();
		//
		////
	protected:
		void calc(); ///< this is called automatically when all frames are received
		void updatePreview();
		///Captures is only used if this payload specifies
		///	that it is offline using isOffline()
		///Offline denotes it performs image analysis at the
		///	end of all captures rather than during captures
		vector<ofPixels> captures;
		ofPixels mean;

		ofPixels_<uint> data;
		ofImage preview;
		bool hasData;
	};
}