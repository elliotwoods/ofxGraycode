#pragma once

#include "BaseCodec.h"
#include "ofBaseTypes.h"
#include "Payload/Base.h"

namespace ofxGraycode {
	class Decoder : public BaseCodec, public ofBaseDraws {
	public:
		void clear() override;
		void clearCaptures(); ///< keep data but drop captures
		void operator<<(const ofPixels& pixels);
		void operator<<(ofBaseHasPixels& image);
		bool hasData() const;

		uint32_t size() const; ///< camera image size
		const vector<ofPixels>& getCaptures() const;
		const ofPixels_<uint32_t>& getData() const;
		const ofPixels& getMedian() const;
		void setThreshold(uint8_t distanceThreshold);
		uint8_t getThreshold() const;

		const DataSet& getDataSet() const;
		void setDataSet(const DataSet&);

		const ofPixels& getMean() const {
			ofLogWarning("ofxGraycode") << "getMean is depreciated, please use getMedian";
			return this->getMedian();
		}

		////
		//ofBaseUpdates
		void update();
		//
		////

		////
		//ofBaseDraws
		void draw(float x, float y) const;
		void draw(float x, float y, float w, float h) const;
		float getWidth() const;
		float getHeight() const;
		//
		////

		////
		//previews
		ofImage& getCameraInProjector();
		ofImage& getProjectorInCamera();
		//
		////

		////
		//file actions
		void loadDataSet(const string filename = "", Payload::Type payloadType = Payload::Type::Graycode, bool throwIfPayloadDoesntMatch = false);
		void saveDataSet(const string filename = "");
		void savePreviews();
		// 
		/////
	protected:
		void calc(); ///< this is called automatically when all frames are received
		void updatePreview();
		void updatePreviewTextures();

		///Captures is only used if this payload specifies
		///	that it is offline using isOffline()
		///Offline denotes it performs image analysis at the
		///	end of all captures rather than during captures
		vector<ofPixels> captures;

		DataSet data;
		ofImage projectorInCamera, cameraInProjector;
		bool needPreviewUpdate; ///<to be thread safe, we perform the updates later in the draw function
	};
}