#include "ofxGraycode/ProjectorPixel.h"
namespace ofxGraycode {
	//--------------------------------------------------------------
	ProjectorPixel::ProjectorPixel() {
		this->blank = true;
	}

	//--------------------------------------------------------------
	ProjectorPixel::ProjectorPixel(const ofVec2f& projectorXY, const ofVec2f& cameraXY, uint32_t distance) {
		this->blank = false;
		this->projector = projectorXY;
		this->addCameraFind(cameraXY, distance);
		this->cameraTotal = 0.0f;
		this->weightTotal = 0.0f;
	}

	//--------------------------------------------------------------
	void ProjectorPixel::addCameraFind(const ofVec2f& cameraXY, uint32_t distance) {
		cameraTotal += cameraXY;
		weightTotal += 1.0f / (float)distance;
	}

	//--------------------------------------------------------------
	ofVec2f ProjectorPixel::getCameraMean() const {
		return this->cameraTotal / this->weightTotal;
	}

	//--------------------------------------------------------------
	bool ProjectorPixel::isFound() const {
		return !this->blank;
	}
}