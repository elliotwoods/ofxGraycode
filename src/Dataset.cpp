#include "DataSet.h"

namespace ofxGraycode {

	////
	// Initialisation
	////
	//
	DataSet::DataSet() {
		clear();
	}

	void DataSet::allocate(int captureWidth, int captureHeight) {
		data.allocate(captureWidth, captureHeight, OF_PIXELS_MONO);
		distance.allocate(captureWidth, captureHeight, OF_PIXELS_MONO);
		mean.allocate(captureWidth, captureHeight, OF_PIXELS_MONO);
		active.allocate(captureWidth, captureHeight, OF_PIXELS_MONO);
		data.set(0,0);
		distance.set(0,0);
		mean.set(0,0);
		active.set(0,0);
	}

	void DataSet::clear() {
		data.clear();
		distance.clear();
		mean.clear();
		active.clear();
		distanceThreshold = 20;
		hasData = false;
	}

	void DataSet::calcMean(const vector<ofPixels>& captures) {
		ofPixels_<uint> tempMean;
		tempMean.allocate(captures[0].getWidth(), captures[0].getHeight(), OF_PIXELS_MONO);
		tempMean.set(0, 0);

		const uchar* pixelIn;
		uint* pixelOut;
		for (uint frame=0; frame<captures.size(); frame++) {
			pixelIn = captures[frame].getPixels();
			pixelOut = tempMean.getPixels();
			for (int i=0; i<tempMean.size(); i++)
				*pixelOut++ += *pixelIn++;
		}

		uint* meanIn = tempMean.getPixels();
		uchar* meanOut = this->mean.getPixels();
		for (uint i = 0; i < tempMean.size(); i++)
			*meanOut++ = *meanIn++ / captures.size();
	}
	
	////
	// Accessors
	////
	//
	const ofPixels_<uint>& DataSet::getData() const {
		return this->data;
	}

	ofPixels_<uint>& DataSet::getData() {
		return this->data;
	}

	const ofPixels& DataSet::getMean() const {
		return this->mean;
	}

	ofPixels_<uint>& DataSet::getDistance() {
		return this->distance;
	}

	uchar DataSet::getDistanceThreshold() const {
		return this->distanceThreshold;
	}

	void DataSet::setDistanceThreshold(uchar distanceThreshold) {
		this->distanceThreshold = distanceThreshold;
		applyDistanceThreshold();
	}

	uint DataSet::getWidth() const {
		return data.getWidth();
	}

	uint DataSet::getHeight() const {
		return data.getHeight();
	}

	uint DataSet::size() const {
		return data.size();
	}

	bool DataSet::getHasData() const {
		return this->hasData;
	}

	void DataSet::setHasData(bool hasData) {
		this->hasData = hasData;
	}

	void DataSet::applyDistanceThreshold() {
		uchar* active = this->active.getPixels();
		uint* distance = this->distance.getPixels();
		for (int i=0; i<this->size(); i++)
			*active++ = *distance++ > distanceThreshold;
	}

	
	////
	// File access
	////
	//
	void DataSet::save() const {
		this->save(ofSystemSaveDialog("dataset.sl", "Save ofxGrayCode::DataSet").getPath());
	}

	void DataSet::save(string filename) const {
		if (!hasData) {
			ofLogError() << "ofxGraycode::DataSet::save : cannot save, this set doesn't have data yet";
			return;
		}

		ofBuffer save;
		ofSaveImage(mean, save);
		ofSaveImage(data, save);
		ofSaveImage(distance, save);
		ofSaveImage(active, save);

		save << distanceThreshold;
		save << ofImage(mean);
		save << data;
		dave << distance;
		save << active;
	}
}