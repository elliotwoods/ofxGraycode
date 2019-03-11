#pragma once
#include "ofVec2f.h"
#include <stdint.h>

namespace ofxGraycode {
	namespace Utils {
		struct ProjectorPixel {

			///blank constructor (unused pixel)
			ProjectorPixel();

			///found pixel
			ProjectorPixel(const ofVec2f& projectorXY, const ofVec2f& cameraXY, uint32_t distance);

			void addCameraFind(const ofVec2f& cameraXY, uint32_t distance);
			ofVec2f getCameraMean() const; //get weighted average mean
			bool isFound() const;

			ofVec2f projector;
			ofVec2f cameraTotal;
			float weightTotal;
		private:
			bool blank;
		};
	}
}