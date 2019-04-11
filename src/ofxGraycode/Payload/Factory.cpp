#include "Factory.h"
#include "Graycode.h"
#include "BalancedGraycode.h"

using namespace std;

namespace ofxGraycode {
	namespace Payload {
		//----------
		shared_ptr<Base> make(const Type & type) {
			switch (type) {
			case Type::Graycode:
				return make_shared<Payload::Graycode>();
			case Type::BalancedGraycode:
				return make_shared<Payload::BalancedGraycode>();
			}

			// We arrived here, we failed to find a matching type
			ofLogError("ofxGraycode::Payload") << "Payload type not defined or unavailable, reverting to Graycode payload type";
			return make_shared<Payload::Graycode>();
		}
	}
}