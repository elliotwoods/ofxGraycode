#pragma once

#include "Types.h"
#include "ofMain.h"

namespace ofxGraycode {
	class DataSet {
	public:
		DataSet();
		void allocate(int captureWidth, int captureHeight, int payloadWidth, int payloadHeight);
		void clear();
		void calcMean(const vector<ofPixels>& captures);
		
		const ofPixels_<uint>& getData() const;
		ofPixels_<uint>& getData();
		const ofPixels& getMean() const;
		ofPixels_<uint>& getDistance();
		const ofPixels& getActive();
		uchar getDistanceThreshold() const;
		void setDistanceThreshold(uchar distanceThreshold);
		uint getWidth() const;
		uint getHeight() const;
		uint getPayloadWidth() const;
		uint getPayloadHeight() const;
		uint size() const;
		bool getHasData() const;
		void setHasData(bool hasData);
		void applyDistanceThreshold();

		void save() const;
		void save(const string filename) const;
		void load();
		void load(const string filename);
		const string& getFilename() const;

	protected:
		ofPixels_<uint> data; ///<encoded pixel index in camera space
		ofPixels mean; ///< used for offline DataSets
		ofPixels_<uint> distance; ///<distance from threshold
		ofPixels active; ///< We treat this as if it stores bools, but no reason to make a bool type really

		uint payloadWidth, payloadHeight;
		uint distanceThreshold;
		bool hasData;
		string filename;
	};
}