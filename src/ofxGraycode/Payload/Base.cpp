#include "Base.h"

using namespace std;

namespace ofxGraycode {
	namespace Payload {
		//----------
		Base::Base() {
			this->allocated = false;
			this->width = 0;
			this->height = 0;
			this->size = 0;
			this->frameCount = 0;
		}

		//----------
		void Base::init(uint32_t width, uint32_t height) {
			this->width = width;
			this->height = height;
			this->size = width*height;
			this->prepare();
			this->allocated = true;
		}

		//----------
		bool Base::isAllocated() const {
			return this->allocated;
		}

		//----------
		uint32_t Base::getWidth() const {
			return this->width;
		}

		//----------
		uint32_t Base::getHeight() const {
			return this->height;
		}

		//----------
		uint32_t Base::getSize() const {
			return this->size;
		}

		//----------
		uint32_t Base::getFrameCount() const {
			return this->frameCount;
		}

		//----------
		bool Base::matchingPixels(const ofPixels& pixels) const {
			return pixels.getWidth() == this->getWidth() &&
				pixels.getHeight() == this->getHeight() &&
				pixels.getImageType() == this->getImageType();
		}

		//----------
		void Base::allocatePixels(ofPixels& pixels) const {
			pixels.allocate(this->getWidth(), this->getHeight(), this->getImageType());
		}
	}
}