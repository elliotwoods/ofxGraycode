#pragma once

#include "ofMain.h"
#include "Utils/ProjectorPixel.h"

#pragma warning ( disable : 4413)
namespace ofxGraycode {
	class DataSet {
	public:
		struct const_iterator {
			struct reference {
				reference(const const_iterator & iterator) :
				camera(iterator.camera),
				projector(*iterator.projector),
				active(*iterator.active),
				median(*iterator.median),
				distance(*iterator.distance),
				dataSet(*iterator.dataSet) { }

				ofVec2f getCameraXY() const;
				ofVec2f getCameraXYNorm() const;
				
				ofVec2f getProjectorXY() const;
				ofVec2f getProjectorXYNorm() const;

				const uint32_t & camera;
				const uint32_t & projector;
				const uint8_t & active;
				const uint8_t & median;
				const uint32_t & distance;
				const DataSet & dataSet;
			};

			friend struct reference;
			friend class DataSet;

			void operator++();
			void operator--();
			bool operator==(const DataSet::const_iterator & other) const;
			bool operator!=(const DataSet::const_iterator & other) const;
			reference operator->() const;
			reference operator*() const;
		protected:
			uint32_t camera;
			const uint32_t * projector;
			const uint8_t * active;
			const uint8_t * median;
			const uint32_t * distance;
			const DataSet* dataSet;
		};

		DataSet();
		void allocate(int captureWidth, int captureHeight, int payloadWidth, int payloadHeight);
		void clear();
		void calcMedian(const vector<ofPixels>& captures);
		void calc(); ///<calc and apply threshold, calc inverse
		
		const ofPixels_<uint32_t>& getData() const;
		ofPixels_<uint32_t>& getData();
		const ofPixels_<uint32_t>& getDataInverse() const;
		ofPixels_<uint32_t>& getDataInverse();
		const ofPixels& getMedian() const;
        const ofPixels& getMedianInverse() const;
		const ofPixels_<uint32_t>& getDistance() const;
		ofPixels_<uint32_t>& getDistance();
		const ofPixels& getActive() const;
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

		vector<Utils::ProjectorPixel> getProjectorPixels() const;
		map<uint32_t, DataSet::const_iterator> getMapping() const; ///<projector pixel as index. highest distance
		
		DataSet::const_iterator begin() const;
		DataSet::const_iterator end() const;

		///Remap an image in camera space to an image in projector space
		template<typename PixelType>
		ofPixels_<PixelType> remapCameraToProjector(const ofPixels_<PixelType> & pixelsInCameraSpace) const {
			ofPixels_<PixelType> pixelsInProjectorSpace;
			pixelsInProjectorSpace.allocate(this->getPayloadWidth()
				, this->getPayloadHeight()
				, pixelsInCameraSpace.getImageType());
			pixelsInProjectorSpace.set(0);
			const auto pixelCount = this->getPayloadSize();
			const auto numChannels = pixelsInCameraSpace.getNumChannels();
			
			for (const auto pixel : *this) {
				if (pixel.active) {
					memcpy(pixelsInProjectorSpace.getData() + numChannels * pixel.projector
						, pixelsInCameraSpace.getData() + numChannels * pixel.camera
						, numChannels * sizeof(PixelType));
				}
			}

			return pixelsInProjectorSpace;
		}

	protected:
		void calcInverse(); //calculate data in projection space
		ofPixels_<uint32_t> data; ///<projector pixel index in camera space
		ofPixels_<uint32_t> dataInverse; ///<camera pixel index in projector space
		ofPixels median; ///< median brightness per camera pixel
		ofPixels medianInverse; ///< median brightness per projector pixel as seen in camera
		ofPixels_<uint32_t> distance; ///<distance from threshold
		ofPixels active; ///< We treat this as if it stores bools, but no reason to make a bool type really

		uint32_t payloadWidth, payloadHeight;
		uint32_t distanceThreshold;
		bool hasData;
		string filename;
	};
}