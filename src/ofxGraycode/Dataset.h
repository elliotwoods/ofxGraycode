#pragma once

#include "ofMain.h"
#include "Correspondence.h"

namespace ofxGraycode {
	class DataSet {
	public:
		DataSet();
		void allocate(int captureWidth, int captureHeight, int payloadWidth, int payloadHeight);
		void clear();
		void calcMean(const vector<ofPixels>& captures);
		void calc(); ///<calc and apply threshold, calc inverse
		
		const ofPixels_<uint32_t>& getData() const;
		ofPixels_<uint32_t>& getData();
		const ofPixels_<uint32_t>& getDataInverse() const;
		ofPixels_<uint32_t>& getDataInverse();
		const ofPixels& getMean() const;
		ofPixels_<uint32_t>& getDistance();
		const ofPixels& getActive();
		uint8_t getDistanceThreshold() const;

		void setDistanceThreshold(uint8_t distanceThreshold);
		uint32_t getWidth() const;
		uint32_t getHeight() const;
		uint32_t getPayloadWidth() const;
		uint32_t getPayloadHeight() const;
		uint32_t getPayloadSize() const;
		uint32_t size() const;
		bool getHasData() const;
		void setHasData(bool hasData);

		void save(string filename="");
		void load(string filename="");
		const string& getFilename() const;

		vector<Correspondence> getCorrespondencesVector() const;
		void saveCorrespondences(string filename="") const;
	protected:
		void calcInverse(); //calculate data in projection space
		ofPixels_<uint32_t> data; ///<projector pixel index in camera space
		ofPixels_<uint32_t> dataInverse; ///<camera pixel index in projector space
		ofPixels mean; ///< used for offline DataSets
		ofPixels_<uint32_t> distance; ///<distance from threshold
		ofPixels active; ///< We treat this as if it stores bools, but no reason to make a bool type really

		uint32_t payloadWidth, payloadHeight;
		uint32_t distanceThreshold;
		bool hasData;
		string filename;
	};
}