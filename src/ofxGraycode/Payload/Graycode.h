#pragma once

#include "Base.h"

namespace ofxGraycode {
	namespace Payload {
		class Graycode : public Offline {
		public:
			Type getType() const override;
			ofImageType getImageType() const;
			void fillPixels(const unsigned int frame, ofPixels& pixels) const;
			void calc(const std::vector<ofPixels> &captures, DataSet &data) const;
			int getFrameCountX() const { return this->frameCountX; }
			int getFrameCountY() const { return this->frameCountY; }
		protected:
			void prepare() override;

			uint32_t getMaxIndex();

			uint32_t frameCountX, frameCountY;

			ofPixels_<uint32_t> data;
			std::vector<uint32_t> dataInverse;
		};
	}
}