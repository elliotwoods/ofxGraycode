#pragma once
#include "Payload/Base.h"
#include "DataSet.h"
#include <memory>

namespace ofxGraycode {
	class BaseCodec {
	public:
		BaseCodec();
		void init(shared_ptr<Payload::Base> payload);
		virtual void reset() = 0;
		int getFrame() const;
		int getFrameCount() const;

		shared_ptr<Payload::Base> getPayload() const;
	protected:
		shared_ptr<Payload::Base> payload;
		int	frame;
	};
}