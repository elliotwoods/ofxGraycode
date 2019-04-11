#pragma once

#include "Base.h"

namespace ofxGraycode {
	namespace Payload {
		std::shared_ptr<Base> make(const Type & = Type::Undefined);
	}
}