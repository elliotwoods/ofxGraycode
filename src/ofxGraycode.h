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

	class Encoder {
	public:
		Encoder();
		void init(Payload& payload);
		void reset();

		bool operator>>(ofPixels& pixels);
		bool operator>>(ofImage& image);

	private:
		Payload* payload;
		int	frame;
	};
}