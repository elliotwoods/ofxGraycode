#include "BalancedGraycode.h"
#include "../Dataset.h"
#include "../Utils/Math.h"

namespace ofxGraycode {
	namespace Payload {
		//----------
		Type BalancedGraycode::getType() const {
			return Type::BalancedGraycode;
		}

		//----------
		ofImageType BalancedGraycode::getImageType() const {
			return OF_IMAGE_GRAYSCALE;
		}

		//----------
		void BalancedGraycode::fillPixels(const unsigned int frame, ofPixels& pixels) const {
			if (!this->isAllocated()) {
				ofLogError("ofxGraycode::Payload::BalancedGraycode") << "fillPixels : cannot encode, we are not allocated";
				return;
			}

			uint8_t* pixel = pixels.getPixels();
			const uint32_t* messageBits = this->messageBits.getPixels();

			const uint32_t halfFrame = frame / 2;

			for (int i = 0; i < size; i++) {
				bool value = (*messageBits++ & (uint32_t)1 << halfFrame) != 0;

				// Odd frames are inverted
				value ^= frame % 2 == 1;

				*pixel++ = value
					? (uint8_t)255
					: (uint8_t)0;
			}
		}

		//----------
		void BalancedGraycode::calc(const vector<ofPixels> &captures, DataSet &data) const {
			//calculate median (unused, just for photo purposes)
			data.calcMedian(captures);

			ofPixels_<uint32_t> distanceAccumulator;
			distanceAccumulator.allocate(data.getWidth(), data.getHeight(), ofPixelFormat::OF_PIXELS_GRAY);
			distanceAccumulator.set(0, 0);

			data.getData().set(0, 0);

			if (captures.size() < this->frameCount) {
				ofLogError("ofxGraycode::Payload::BalancedGraycode") << "Cannot perform calc until all frames are available";
				return;
			}

			//decode
			auto halfFrameCount = this->frameCount / 2;
			for (uint32_t framePairIndex = 0; framePairIndex < halfFrameCount; framePairIndex++) {
				auto pixelA = captures[framePairIndex * 2].getData();
				auto pixelB = captures[framePairIndex * 2 + 1].getData();
				auto distanceAccumulatorPixel = distanceAccumulator.getData();
				auto dataOut = data.getData().getData();

				for (uint32_t i = 0; i < data.size(); i++, pixelA++, pixelB++, distanceAccumulatorPixel++, dataOut++) {
					auto high = *pixelA > *pixelB;

					if (high) {
						*dataOut |= (uint32_t)1 << framePairIndex;
					}
					else {
						//currently we presume data defaults to 0, since we set to 0 before we begin
					}

					auto distance = high
						? *pixelA - *pixelB
						: *pixelB - *pixelA;

					*distanceAccumulatorPixel += distance;
				}
			}

			//calculate distance values
			{
				auto distanceAccumulatorPixel = distanceAccumulator.getData();
				auto distancePixel = data.getDistance().getData();
				for (uint32_t i = 0; i < distanceAccumulator.size(); i++) {
					*distancePixel++ = *distanceAccumulatorPixel++ / halfFrameCount;
				}
			}

			//decode data
			{
				auto dataOut = data.getData().getData();
				for (int i = 0; i < data.size(); i++, dataOut++) {
					*dataOut = this->messageBitsInverse[*dataOut];
				}
			}

			//apply threshold, invert into projector space
			//note that for balanced graycode, the 'distance' is the minimum distance across all the frame pairs
			data.calc();

			data.setHasData(true);
		}

		//----------
		void BalancedGraycode::prepare() {
			this->frameCountX = ceil(log((float)this->width) / log((float)2));
			this->frameCountY = ceil(log((float)this->height) / log((float)2));
			this->frameCount = (this->frameCountX + this->frameCountY) * 2;

			this->messageBits.allocate(this->width, this->height, OF_IMAGE_GRAYSCALE);
			this->messageBitsInverse.resize(getMaxIndex());

			uint32_t* pix = this->messageBits.getPixels();
			uint32_t idx = 0;

			for (uint32_t y = 0; y < height; y++) {
				for (uint32_t x = 0; x < width; x++, pix++, idx++) {
					*pix = Utils::Math::toGraycode(x) +
						(Utils::Math::toGraycode(y) << (int)frameCountX);
					this->messageBitsInverse[*pix] = idx;
				}
			}

			this->framesReceived = 0;
		}

		//----------
		uint32_t BalancedGraycode::getMaxIndex() {
			return 1 << (frameCountX + frameCountY);
		}
	}
}