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
		dataInverse.allocate(payloadWidth, payloadHeight, OF_PIXELS_MONO);
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
		dataInverse.clear();
		distance.clear();
		mean.clear();
		active.clear();
		distanceThreshold = 10;
		hasData = false;
		filename = "";
		payloadWidth = 0;
		payloadHeight = 0;
	}

	void DataSet::calcMean(const vector<ofPixels>& captures) {
		ofPixels_<uint32_t> tempMean;
		tempMean.allocate(captures[0].getWidth(), captures[0].getHeight(), OF_PIXELS_MONO);
		tempMean.set(0, 0);

		const uint8_t* pixelIn;
		uint32_t* pixelOut;
		for (uint32_t frame=0; frame<captures.size(); frame++) {
			pixelIn = captures[frame].getPixels();
			pixelOut = tempMean.getPixels();
			for (int i=0; i<tempMean.size(); i++)
				*pixelOut++ += *pixelIn++;
		}

		uint32_t* meanIn = tempMean.getPixels();
		uint8_t* meanOut = this->mean.getPixels();
		for (uint32_t i = 0; i < tempMean.size(); i++)
			*meanOut++ = *meanIn++ / captures.size();
	}
	
	void DataSet::calc() {
		uint8_t* active = this->active.getPixels();
		uint32_t* distance = this->distance.getPixels();
		for (int i=0; i<this->size(); i++)
			*active++ = *distance++ > distanceThreshold;
		calcInverse();
	}

	////
	// Accessors
	////
	//
	const ofPixels_<uint32_t>& DataSet::getData() const {
		return this->data;
	}

	ofPixels_<uint32_t>& DataSet::getData() {
		return this->data;
	}

	const ofPixels_<uint32_t>& DataSet::getDataInverse() const {
		return this->dataInverse;
	}

	ofPixels_<uint32_t>& DataSet::getDataInverse() {
		return this->dataInverse;
	}

	const ofPixels& DataSet::getMean() const {
		return this->mean;
	}

	ofPixels_<uint32_t>& DataSet::getDistance() {
		return this->distance;
	}

	const ofPixels& DataSet::getActive() {
		return this->active;
	}

	uint8_t DataSet::getDistanceThreshold() const {
		return this->distanceThreshold;
	}

	void DataSet::setDistanceThreshold(uint8_t distanceThreshold) {
		this->distanceThreshold = distanceThreshold;
		calc();
	}

	uint32_t DataSet::getWidth() const {
		return data.getWidth();
	}

	uint32_t DataSet::getHeight() const {
		return data.getHeight();
	}

	uint32_t DataSet::getPayloadWidth() const {
		return this->payloadWidth;
	}

	uint32_t DataSet::getPayloadHeight() const {
		return this->payloadHeight;
	}

	uint32_t DataSet::getPayloadSize() const {
		return this->payloadWidth * this->payloadHeight;
	}

	uint32_t DataSet::size() const {
		return data.size();
	}

	bool DataSet::getHasData() const {
		return this->hasData;
	}

	void DataSet::setHasData(bool hasData) {
		this->hasData = hasData;
	}
	
	////
	// File access
	////
	//

	void DataSet::save(string filename) {
		if (!hasData) {
			ofLogError() << "ofxGraycode::DataSet::save : cannot save, this set doesn't have data yet";
			return;
		}

		if (filename=="")
			filename = ofSystemSaveDialog("dataset.sl", "Save ofxGrayCode::DataSet").getPath();

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
		save.write((char*)data.getPixels(), this->size() * sizeof(uint32_t));
		save.write((char*)mean.getPixels(), this->size() * sizeof(uint8_t));
		save.write((char*)distance.getPixels(), this->size() * sizeof(uint32_t));
		save.write((char*)active.getPixels(), this->size() * sizeof(uint8_t));
		save.close();
	}

	void DataSet::load(string filename) {
		if (filename=="")
			filename = ofSystemLoadDialog("Load ofxGrayCode::DataSet").getPath();

		this->filename = filename;

		uint32_t size;
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

		load.read((char*)data.getPixels(), this->size() * sizeof(uint32_t));
		load.read((char*)mean.getPixels(), this->size() * sizeof(uint8_t));
		load.read((char*)distance.getPixels(), this->size() * sizeof(uint32_t));
		load.read((char*)active.getPixels(), this->size() * sizeof(uint8_t));
		load.close();

		this->hasData = true;
		this->calc();
	}

	const string& DataSet::getFilename() const {
		return this->filename;
	}

	vector<Correspondence> DataSet::getCorrespondencesVector() const {
		vector<Correspondence> correspondences;
		if (!hasData) {
			ofLogError("ofxGraycode::DataSet") << "Cannot get correspondences vector as we have no data yet";
			return correspondences;
		}
		const uint8_t* active = this->active.getPixels();
		const uint32_t* data = this->data.getPixels();
		for (uint32_t i=0; i<size(); i++, active++, data++) {
			if (*active)
				correspondences.push_back(Correspondence(i, *data));
		}
		return correspondences;
	}

	void DataSet::saveCorrespondences(string filename) const {
		if (!hasData) {
			ofLogError("ofxGraycode::DataSet") << "Cannot save correspondences vector as we have no data yet";
			return;
		}

		if (filename=="") {
			filename = this->getFilename();
			if (filename=="")
				filename = ofSystemSaveDialog("sl.correspondences", "Save correspondences").getPath();
			else
				filename += ".correspondences";
		}

		vector<Correspondence> correspondences = this->getCorrespondencesVector();
		ofstream fileOut;
		try {
			fileOut.open(ofToDataPath(filename, true), ios::binary);
			uint32_t size = correspondences.size();
			fileOut.write((char*)&size, sizeof(uint32_t));
			vector<Correspondence>::iterator it;
			for (it = correspondences.begin(); it != correspondences.end(); it++)
				fileOut.write((char*)&(*it), sizeof(Correspondence));
			fileOut.close();
		} catch (...) {
			ofLogError("ofxGraycode") << "Save correspondences file write failed";
		}
	}

	void DataSet::calcInverse() {
		this->dataInverse.set(0, 0);
		uint32_t *dataInverse = this->dataInverse.getPixels();
		uint32_t *data = this->data.getPixels();
		uint8_t *active = this->active.getPixels();
		uint32_t payloadSize = this->getPayloadSize();
		for (uint32_t i=0; i<this->data.size(); i++, data++, active++) {
			if (*data < payloadSize && *active)
				dataInverse[*data] = i;
		}
	}
}