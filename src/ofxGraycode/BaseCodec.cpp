#include "BaseCodec.h"

namespace ofxGraycode {
	//----------
	BaseCodec::BaseCodec() {
		this->frame = 0;
		this->payload = 0;
	}

	//----------
	void BaseCodec::init(shared_ptr<Payload::Base> payload) {
		this->payload = payload;
		reset();
	}

	//----------
	int BaseCodec::getFrame() const {
		return this->frame;
	}

	//----------
	int BaseCodec::getFrameCount() const {
		return this->payload->getFrameCount();
	}

	//----------
	shared_ptr<Payload::Base> BaseCodec::getPayload() const {
		return this->payload;
	}
}