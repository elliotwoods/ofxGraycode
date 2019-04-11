#pragma once

// Payloads are what defines an encoding
// Changing the payload results in a different message being
//	sent and decoded.
// Offline payloads require that all the frames are kept until
//	the end.
// Online codecs (e.g. balanced which uses dynamic threshold
//	creation), do not need to work on all the frames at the end
//	all at once

#include <stdint.h>
#include "ofPixels.h"

namespace ofxGraycode {
	class DataSet;

	namespace Payload {
		//for factory use
		enum class Type {
			Undefined,
			Graycode,
			BalancedGraycode
		};

		class Base {
		public:
			Base();
			virtual Type getType() const = 0;
			void init(uint32_t width, uint32_t height);

			bool isAllocated() const;

			///Determines whether the payload
			///	needs the decoder to keep a
			///	copy of all the captures for
			///	the end
			virtual bool isOffline() const { return false; }

			uint32_t getWidth() const;
			uint32_t getHeight() const;
			uint32_t getSize() const;
			uint32_t getFrameCount() const;
			virtual ofImageType getImageType() const = 0;

			bool matchingPixels(const ofPixels& pixels) const;
			void allocatePixels(ofPixels& pixels) const;
			virtual void fillPixels(const unsigned int frame, ofPixels& pixels) const = 0;
			virtual void readPixels(const unsigned int frame, const ofPixels& pixels) { };

		protected:
			virtual void prepare() = 0;

			bool allocated;
			uint32_t width, height;
			uint32_t size;
			uint32_t frameCount;
		};
	
		class Offline : public Base {
		public:
			bool isOffline() const { return true; }
			virtual void calc(const std::vector<ofPixels> &captures, DataSet &data) const = 0;
		};

		class Online : public Base {
		public:
			bool isOffline() const { return false; }
			virtual void calc(DataSet& data) const = 0;
		};
	}
}