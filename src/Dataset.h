#pragma once

#include "Types.h"
#include "ofMain.h"

namespace ofxGraycode {
	class DataSet {
	public:
		DataSet();
		void allocate(int captureWidth, int captureHeight);
		void clear();
		void calcMean(const vector<ofPixels>& captures);
		
		const ofPixels_<uint>& getData() const;
		ofPixels_<uint>& getData();
		const ofPixels& getMean() const;
		ofPixels_<uint>& getDistance();
		uchar getDistanceThreshold() const;
		void setDistanceThreshold(uchar distanceThreshold);
		uint getWidth() const;
		uint getHeight() const;
		uint size() const;
		bool getHasData() const;
		void setHasData(bool hasData);
		void applyDistanceThreshold();

		void save() const;
		void save(string filename) const;
	protected:
		ofPixels mean; ///< used for offline DataSets
		ofPixels_<uint> data; ///<encoded pixel index in camera space
		ofPixels_<uint> distance; ///<distance from threshold
		ofPixels active; ///< We treat this as if it stores bools, but no reason to make a bool type really

		uint distanceThreshold;
		bool hasData;
	};
}