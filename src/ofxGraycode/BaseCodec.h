#pragma once
#include "Payload/Base.h"
#include "DataSet.h"
#include <memory>

namespace ofxGraycode {
	class BaseCodec {
	public:
		BaseCodec();
		void init(shared_ptr<Payload::Base> payload);
		virtual void clear() { }
		int getFrame() const;
		int getFrameCount() const;

		shared_ptr<Payload::Base> getPayload() const;
		void setPayload(shared_ptr<Payload::Base>);
	protected:
		shared_ptr<Payload::Base> payload;
		int	frame;
	};
}