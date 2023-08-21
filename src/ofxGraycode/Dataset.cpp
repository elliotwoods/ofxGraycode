#include "DataSet.h"

// Bit patterns for when saving
#define OFXGRAYCODE_DATASET_HAS_DATA 1 << 0
#define OFXGRAYCODE_DATASET_HAS_DATAINVERSE 1 << 1
#define OFXGRAYCODE_DATASET_HAS_MEDIAN 1 << 2
#define OFXGRAYCODE_DATASET_HAS_DISTANCE 1 << 3
#define OFXGRAYCODE_DATASET_HAS_ACTIVE 1 << 4
#define OFXGRAYCODE_DATASET_HAS_MEDIAN_INVERSE 1 << 5

namespace ofxGraycode {
#pragma mark DataSet::iterator
	//----------
	glm::vec2 DataSet::const_iterator::reference::getCameraXY() const {
		ofVec2f xy;
		xy.x = this->camera % this->dataSet.getWidth();
		xy.y = this->camera / this->dataSet.getWidth();
		return xy;
	}

	//----------
	glm::vec2 DataSet::const_iterator::reference::getCameraXYNorm() const {
		glm::vec2 xy = this->getCameraXY();
		xy /= ofVec2f(this->dataSet.getWidth(), this->dataSet.getHeight());
		xy *= 2.0f; //0..2
		xy -= 1.0f;
		xy.y = -xy.y;
		return xy;
	}

	//----------
	glm::vec2 DataSet::const_iterator::reference::getProjectorXY() const {
		glm::vec2 xy;
		xy.x = this->projector % this->dataSet.getPayloadWidth();
		xy.y = this->projector / this->dataSet.getPayloadWidth();
		return xy;
	}

	//----------
	glm::vec2 DataSet::const_iterator::reference::getProjectorXYNorm() const {
		glm::vec2 xy = this->getProjectorXY();
		xy /= ofVec2f(this->dataSet.getPayloadWidth(), this->dataSet.getPayloadHeight());
		xy *= 2.0f; //0..2
		xy -= 1.0f;
		xy.y = -xy.y;
		return xy;
	}

	//----------
	void DataSet::const_iterator::operator++() {
		this->camera++;
		this->projector++;
		this->active++;
		this->median++;
		this->distance++;
	}

	//----------
	void DataSet::const_iterator::operator--() {
		this->camera--;
		this->projector--;
		this->active--;
		this->median--;
		this->distance--;
	}

	//----------
	bool DataSet::const_iterator::operator==(const DataSet::const_iterator & other) const {
		//compare pointers
		return this->projector == other.projector;
	}

	//----------
	bool DataSet::const_iterator::operator!=(const DataSet::const_iterator & other) const {
		//compare pointers
		return this->projector != other.projector;
	}

	//----------
	DataSet::const_iterator::reference DataSet::const_iterator::operator->() const {
		return DataSet::const_iterator::reference(*this);
	}

	//----------
	DataSet::const_iterator::reference DataSet::const_iterator::operator*() const {
		return DataSet::const_iterator::reference(*this);
	}

#pragma mark DataSet
	//----------
	DataSet::DataSet() {
		clear();
	}

	//----------
	void DataSet::allocate(int captureWidth, int captureHeight, int payloadWidth, int payloadHeight) {
		data.allocate(captureWidth, captureHeight, OF_PIXELS_MONO);
		dataInverse.allocate(payloadWidth, payloadHeight, OF_PIXELS_MONO);
		distance.allocate(captureWidth, captureHeight, OF_PIXELS_MONO);
		median.allocate(captureWidth, captureHeight, OF_PIXELS_MONO);
		medianInverse.allocate(payloadWidth, payloadHeight, OF_PIXELS_MONO);
		active.allocate(captureWidth, captureHeight, OF_PIXELS_MONO);
		data.set(0, 0);
		distance.set(0, 0);
		median.set(0, 0);
		active.set(0, 0);
		this->payloadWidth = payloadWidth;
		this->payloadHeight = payloadHeight;
	}

	//----------
	void DataSet::clear() {
		data.clear();
		dataInverse.clear();
		distance.clear();
		median.clear();
		active.clear();
		distanceThreshold = 10;
		hasData = false;
		filename = "";
		payloadWidth = 0;
		payloadHeight = 0;
	}

	//----------
	void DataSet::calcMedian(const vector<ofPixels>& captures) {
		ofPixels tempMax;
		ofPixels tempMin;

		tempMin.allocate(captures[0].getWidth(), captures[0].getHeight(), OF_PIXELS_MONO);
		tempMax.allocate(captures[0].getWidth(), captures[0].getHeight(), OF_PIXELS_MONO);

		tempMin.set(0, 255);
		tempMax.set(0, 0);

		const uint8_t* pixelIn;
		uint8_t* pixelMax;
		uint8_t* pixelMin;

		uint32_t size = captures[0].getWidth() * captures[0].getHeight();

		for (uint32_t frame = 0; frame < captures.size(); frame++) {
			pixelIn = captures[frame].getData();
			pixelMin = tempMin.getData();
			pixelMax = tempMax.getData();
			for (int i = 0; i < size; i++) {
				if (*pixelIn > *pixelMax) {
					*pixelMax = *pixelIn;
				}
				if (*pixelIn < *pixelMin) {
					*pixelMin = *pixelIn;
				}
				pixelIn++;
				pixelMin++;
				pixelMax++;
			}
		}

		pixelMin = tempMin.getData();
		pixelMax = tempMax.getData();

		uint8_t* medianOut = this->median.getData();
		for (uint32_t i = 0; i < size; i++) {
			*medianOut++ = (uint8_t)(((uint16_t)(*pixelMax++) + (uint16_t)(*pixelMin++)) / 2);
		}
	}

	void DataSet::calc() {
		uint8_t* active = this->active.getData();
		uint32_t* distance = this->distance.getData();
		for (int i = 0; i < this->size(); i++) {
			*active++ = *distance++ > distanceThreshold ? 255 : 0;
		}
		calcInverse();
	}

	////
	// Accessors
	////
	//
	//----------
	const ofPixels_<uint32_t>& DataSet::getData() const {
		return this->data;
	}

	//----------
	ofPixels_<uint32_t>& DataSet::getData() {
		return this->data;
	}

	//----------
	const ofPixels_<uint32_t>& DataSet::getDataInverse() const {
		return this->dataInverse;
	}

	//----------
	ofPixels_<uint32_t>& DataSet::getDataInverse() {
		return this->dataInverse;
	}

	//----------
	const ofPixels& DataSet::getMedian() const {
		return this->median;
	}

	//----------
	const ofPixels& DataSet::getMedianInverse() const {
		return this->medianInverse;
	}

	//----------
	const ofPixels_<uint32_t>& DataSet::getDistance() const {
		return this->distance;
	}

	//----------
	ofPixels_<uint32_t>& DataSet::getDistance() {
		return this->distance;
	}

	//----------
	const ofPixels& DataSet::getActive() const {
		return this->active;
	}

	//----------
	uint8_t DataSet::getDistanceThreshold() const {
		return this->distanceThreshold;
	}

	//----------
	void DataSet::setDistanceThreshold(uint8_t distanceThreshold) {
		this->distanceThreshold = distanceThreshold;
		if (this->getHasData()) {
			calc();
		}
	}

	//----------
	uint32_t DataSet::getWidth() const {
		return data.getWidth();
	}

	//----------
	uint32_t DataSet::getHeight() const {
		return data.getHeight();
	}

	uint32_t DataSet::getPayloadWidth() const {
		return this->payloadWidth;
	}

	//----------
	uint32_t DataSet::getPayloadHeight() const {
		return this->payloadHeight;
	}

	//----------
	uint32_t DataSet::getPayloadSize() const {
		return this->payloadWidth * this->payloadHeight;
	}

	//----------
	uint32_t DataSet::size() const {
		return data.size();
	}

	//----------
	bool DataSet::getHasData() const {
		return this->hasData;
	}

	//----------
	void DataSet::setHasData(bool hasData) {
		this->hasData = hasData;
	}

	////
	// File access
	////
	//

	//----------
	void DataSet::save(string filename) {
		if (!hasData) {
			ofLogError() << "ofxGraycode::DataSet::save : cannot save, this set doesn't have data yet";
			return;
		}

		if (filename == "") {
			filename = ofSystemSaveDialog("dataset.sl", "Save ofxGrayCode::DataSet").getPath();
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
		save << width << "\t" << height << endl;
		save << payloadWidth << "\t" << payloadHeight << endl;
		save << this->distanceThreshold << endl;

		uint32_t contained = 0;
		contained |= OFXGRAYCODE_DATASET_HAS_DATA;
		contained |= OFXGRAYCODE_DATASET_HAS_DATAINVERSE;
		contained |= OFXGRAYCODE_DATASET_HAS_MEDIAN;
		contained |= OFXGRAYCODE_DATASET_HAS_MEDIAN_INVERSE;
		contained |= OFXGRAYCODE_DATASET_HAS_DISTANCE;
		contained |= OFXGRAYCODE_DATASET_HAS_ACTIVE;
		save.write((char*)&contained, sizeof(contained));

		save.write((char*)data.getData(), this->size() * sizeof(uint32_t));
		save.write((char*)dataInverse.getData(), this->getPayloadSize() * sizeof(uint32_t));
		save.write((char*)median.getData(), this->size() * sizeof(uint8_t));
		save.write((char*)medianInverse.getData(), this->getPayloadSize() * sizeof(uint8_t));
		save.write((char*)distance.getData(), this->size() * sizeof(uint32_t));
		save.write((char*)active.getData(), this->size() * sizeof(uint8_t));
		save.close();
	}

	//----------
	void DataSet::load(string filename) {
		if (filename == "")
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

		uint32_t contained;
		load.read((char*)&contained, sizeof(contained));

		if (contained & OFXGRAYCODE_DATASET_HAS_DATA)
			load.read((char*)data.getData(), this->size() * sizeof(uint32_t));
		if (contained & OFXGRAYCODE_DATASET_HAS_DATAINVERSE)
			load.read((char*)dataInverse.getData(), this->getPayloadSize() * sizeof(uint32_t));
		if (contained & OFXGRAYCODE_DATASET_HAS_MEDIAN)
			load.read((char*)median.getData(), this->size() * sizeof(uint8_t));
		if (contained & OFXGRAYCODE_DATASET_HAS_MEDIAN_INVERSE)
			load.read((char*)medianInverse.getData(), this->getPayloadSize() * sizeof(uint8_t));
		if (contained & OFXGRAYCODE_DATASET_HAS_DISTANCE)
			load.read((char*)distance.getData(), this->size() * sizeof(uint32_t));
		if (contained & OFXGRAYCODE_DATASET_HAS_ACTIVE)
			load.read((char*)active.getData(), this->size() * sizeof(uint8_t));
		load.close();

		this->hasData = true;
		this->calc();
	}

	//----------
	const string& DataSet::getFilename() const {
		return this->filename;
	}

	//----------
	vector<Utils::ProjectorPixel> DataSet::getProjectorPixels() const {
		const uint32_t *data = this->data.getData();
		const uint8_t *active = this->active.getData();
		const uint32_t *distance = this->distance.getData();

		vector<Utils::ProjectorPixel> projectorPixels(this->getPayloadSize());
		ofVec2f cameraXY, projectorXY;
		uint32_t activeCount = 0;

		for (int i = 0; i < this->size(); i++, data++, active++, distance++) {
			if (*active) {
				cameraXY = ofVec2f(i % this->getWidth(), i / this->getWidth()) /
					ofVec2f(this->getWidth(), this->getHeight()) * 2.0f - ofVec2f(1.0f, 1.0f);
				if (projectorPixels[*data].isFound())
					projectorPixels[*data].addCameraFind(cameraXY, *distance);
				else {
					projectorXY = ofVec2f(*data % this->getPayloadWidth(), *data / this->getPayloadWidth()) /
						ofVec2f(this->getPayloadWidth(), this->getPayloadHeight()) * 2.0f;
					projectorXY.x -= 1.0f;
					projectorXY.y = 1.0f - projectorXY.y;
					projectorPixels[*data] = Utils::ProjectorPixel(projectorXY, cameraXY, *distance);
					activeCount++;
				}
			}
		}

		//filter blank pixels
		vector<Utils::ProjectorPixel> filteredPixels;
		filteredPixels.reserve(activeCount);
		for (int i = 0; i < this->size(); i++) {
			if (projectorPixels[i].isFound()) {
				filteredPixels.push_back(projectorPixels[i]);
			}
		}

		return filteredPixels;
	}

	//----------
	map<uint32_t, DataSet::const_iterator> DataSet::getMapping() const {
		map<uint32_t, DataSet::const_iterator> mapping; //projector
		map<uint32_t, uint32_t> distance; //projector, distance
		for (DataSet::const_iterator it = this->begin(); it != this->end(); ++it) {
			if (!(*it).active)
				continue;
			if (mapping.count((*it).projector) == 0) {
				mapping.insert(pair<uint32_t, DataSet::const_iterator>((*it).projector, it));
				distance.insert(pair<uint32_t, uint32_t>((*it).projector, (*it).distance));
			}
			else {
				if ((*it).distance > distance.at((*it).projector))
					mapping.at((*it).projector) = it;
			}
		}
		return mapping;
	}

	//----------
	DataSet::const_iterator DataSet::begin() const {
		DataSet::const_iterator it;
		it.camera = 0;
		it.projector = this->data.getData();
		it.active = this->active.getData();
		it.distance = this->distance.getData();
		it.median = this->median.getData();
		it.dataSet = this;
		return it;
	}

	//----------
	DataSet::const_iterator DataSet::end() const {
		DataSet::const_iterator it;
		it.camera = this->size();
		it.projector = this->data.getData() + this->size();
		it.active = this->active.getData() + this->size();
		it.distance = this->distance.getData() + this->size();
		it.median = this->median.getData() + this->size();
		it.dataSet = this;
		return it;
	}

	//----------
	void DataSet::calcInverse() {
		this->dataInverse.set(0, 0);
		uint32_t *dataInverse = this->dataInverse.getData();
		uint8_t *medianInverse = this->medianInverse.getData();
		uint32_t *data = this->data.getData();
		uint8_t *active = this->active.getData();
		uint32_t payloadSize = this->getPayloadSize();

		memset(medianInverse, 0, payloadSize);

		for (uint32_t i = 0; i < this->data.size(); i++, data++, active++) {
			if (*data < payloadSize && *active) {
				dataInverse[*data] = i;
				medianInverse[*data] = median[i];
			}
		}
	}
}