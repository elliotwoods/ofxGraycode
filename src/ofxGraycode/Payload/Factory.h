#pragma once

#include "Base.h"

namespace ofxGraycode {
	namespace Payload {
		shared_ptr<Base> make(const Type & = Type::Undefined);
	}
}