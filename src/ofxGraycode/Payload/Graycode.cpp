#include "Graycode.h"
#include "../Dataset.h"
#include "../Utils/Math.h"

namespace ofxGraycode {
	namespace Payload {
		//----------
		Type Graycode::getType() const {
			return Type::Graycode;
		}

		//----------
		ofImageType Graycode::getImageType() const {
			return OF_IMAGE_GRAYSCALE;
		}

		//----------
		void Graycode::fillPixels(const unsigned int frame, ofPixels& pixels) const {
			if (!this->isAllocated()) {
				ofLogError() << "ofxGraycode::Payload::Graycode::fillPixels : cannot encode, we are not allocated";
				return;
			}

			uint8_t* pixel = pixels.getData();
			const uint32_t* data = this->data.getData();

			for (int i = 0; i < size; i++) {
				bool value = (*data++ & (uint32_t)1 << frame) == (uint32_t)1 << frame;
				*pixel++ = value
					? (uint8_t)255
					: (uint8_t)0;
			}
		}

		//----------
		void Graycode::calc(const vector<ofPixels>& captures, DataSet& data) const {
			if (captures.size() != frameCount)
			{
				ofLogError() << "ofxGraycode::PayloadGraycode::calc : cannot calc as number of captured frames does not match target framecount";
				return;
			}

			//prepare output
			data.calcMedian(captures);

			//decode
			const uint8_t* thresholdIn;
			const uint8_t* pixelIn;
			uint32_t* dataOut;
			uint32_t* distanceOut;
			int distance;
			for (uint32_t frame = 0; frame < frameCount; frame++) {
				pixelIn = captures[frame].getData();
				thresholdIn = data.getMedian().getData();
				dataOut = data.getData().getData();
				distanceOut = data.getDistance().getData();
				for (int i = 0; i < data.size(); i++, dataOut++, pixelIn++, thresholdIn++) {
					distance = (int)*pixelIn - (int)*thresholdIn;
					if (distance > 0)
						*dataOut |= (uint32_t)1 << frame;
					*distanceOut++ += abs(distance);
				}
			}

			//normalise distance
			distanceOut = data.getDistance().getData();
			for (int i = 0; i < data.size(); i++)
				*distanceOut++ /= frameCount;

			//decode data
			dataOut = data.getData().getData();

			for (int i = 0; i < data.size(); i++, dataOut++)
				*dataOut = dataInverse[*dataOut];

			//apply threshold, invert into projector space
			data.calc();

			data.setHasData(true);
		}

		//----------
		void Graycode::prepare() {
			this->frameCountX = ceil(log((float)width) / log((float)2));
			this->frameCountY = ceil(log((float)height) / log((float)2));
			this->frameCount = frameCountX + frameCountY;

			data.allocate(width, height, OF_IMAGE_GRAYSCALE);
			dataInverse.resize(getMaxIndex());

			uint32_t* pix = data.getData();
			uint32_t idx = 0;

			for (uint32_t y = 0; y < height; y++) {
				for (uint32_t x = 0; x < width; x++, pix++, idx++) {
					*pix = Utils::Math::toGraycode(x) +
						(Utils::Math::toGraycode(y) << (int)frameCountX);
					dataInverse[*pix] = idx;
				}
			}
		}

		//----------
		uint32_t Graycode::getMaxIndex() {
			return 1 << (frameCountX + frameCountY);
		}
	}
}