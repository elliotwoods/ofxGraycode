#pragma once

#include "BaseCodec.h"

namespace ofxGraycode {
	class Encoder : public BaseCodec {
	public:
		void reset();

		bool operator >> (ofPixels& pixels);
		bool operator >> (ofImage& image);
	};
}