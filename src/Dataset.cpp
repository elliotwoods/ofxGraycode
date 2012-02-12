#include "DataSet.h"

namespace ofxGraycode {

	////
	// Initialisation
	////
	//
	DataSet::DataSet() {
		clear();
	}

	void DataSet::allocate(int captureWidth, int captureHeight, int payloadWidth, int payloadHeight) {
		data.allocate(captureWidth, captureHeight, OF_PIXELS_MONO);
		distance.allocate(captureWidth, captureHeight, OF_PIXELS_MONO);
		mean.allocate(captureWidth, captureHeight, OF_PIXELS_MONO);
		active.allocate(captureWidth, captureHeight, OF_PIXELS_MONO);
		data.set(0,0);
		distance.set(0,0);
		mean.set(0,0);
		active.set(0,0);
		this->payloadWidth = payloadWidth;
		this->payloadHeight = payloadHeight;
	}

	void DataSet::clear() {
		data.clear();
		distance.clear();
		mean.clear();
		active.clear();
		distanceThreshold = 20;
		hasData = false;
		filename = "";
		payloadWidth = 0;
		payloadHeight = 0;
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

	const ofPixels& DataSet::getActive() {
		return this->active;
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

	uint DataSet::getPayloadWidth() const {
		return this->payloadWidth;
	}

	uint DataSet::getPayloadHeight() const {
		return this->payloadHeight;
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
	void DataSet::save() {
		this->save(ofSystemSaveDialog("dataset.sl", "Save ofxGrayCode::DataSet").getPath());
	}

	void DataSet::save(const string filename) {
		if (!hasData) {
			ofLogError() << "ofxGraycode::DataSet::save : cannot save, this set doesn't have data yet";
			return;
		}

		if (filename=="") {
			ofLogWarning() << "ofxGraycode::DataSet::save failed as no file selected";
			return;
		}

		this->filename = filename;

		int width = this->data.getWidth();
		int height = this->data.getHeight();
		
		ofstream save(ofToDataPath(filename).c_str(), ios::binary);
		if (!save.is_open()) {
			ofLogError() << "ofxGraycode::DataSet::save failed to open file " << filename;
			return;
		}

		save << this->size() << endl;
		save << width << "\t" <<  height << endl;
		save << payloadWidth << "\t" <<  payloadHeight << endl;
		save << this->distanceThreshold << endl;
		save.write((char*)data.getPixels(), this->size() * sizeof(uint));
		save.write((char*)mean.getPixels(), this->size() * sizeof(uchar));
		save.write((char*)distance.getPixels(), this->size() * sizeof(uint));
		save.write((char*)active.getPixels(), this->size() * sizeof(uchar));
		save.close();
	}

	void DataSet::load() {
		this->load(ofSystemLoadDialog("Load ofxGrayCode::DataSet").getPath());
	}

	void DataSet::load(const string filename) {
		if (filename=="") {
			ofLogWarning() << "ofxGraycode::DataSet::load failed as no file selected";
			return;
		}

		this->filename = filename;

		uint size;
		unsigned short width, height;

		ifstream load(ofToDataPath(filename).c_str(), ios::binary);
		if (!load.is_open()) {
			ofLogError() << "ofxGraycode::DataSet::load failed to open file " << filename;
			return;
		}

		load >> size;
		load >> width;
		load >> height;
		load >> payloadWidth;
		load >> payloadHeight;
		load >> this->distanceThreshold;
		load.ignore(1);

		if (size != width * height) {
			ofLogError() << "ofxGraycode::DataSet::load failed. size mismatch in file. is it corrupt / correct type?";
			return;
		}
		this->allocate(width, height, payloadWidth, payloadHeight);

		load.read((char*)data.getPixels(), this->size() * sizeof(uint));
		load.read((char*)mean.getPixels(), this->size() * sizeof(uchar));
		load.read((char*)distance.getPixels(), this->size() * sizeof(uint));
		load.read((char*)active.getPixels(), this->size() * sizeof(uchar));
		load.close();

		this->hasData = true;
	}

	const string& DataSet::getFilename() const {
		return this->filename;
	}
}