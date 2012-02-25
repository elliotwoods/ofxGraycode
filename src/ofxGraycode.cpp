//
//  ofxGraycode.cpp
//  ofxGraycode
//
//  Created by Elliot Woods on 10/11/2011.
//	http://www.kimchiandchips.com
//
#include "ofxGraycode.h"

namespace ofxGraycode {
	
//----------------------------------------
// BaseCodec
	BaseCodec::BaseCodec() {
		this->frame = 0;
		this->payload = 0;
	}

	void BaseCodec::init(const Payload& payload) {
		this->payload = &payload;
		reset();
	}

	int BaseCodec::getFrame() {
		return this->frame;
	}

//----------------------------------------
// Encoder
	void Encoder::reset() {
		frame = 0;
	}

	bool Encoder::operator>>(ofPixels& pixels) {

		if (frame >= payload->getFrameCount())
			return false;

		if (!payload->matchingPixels(pixels))
			payload->allocatePixels(pixels);

		payload->fillPixels(frame++, pixels);

		return true;
	}

	bool Encoder::operator>>(ofImage& image) {
		if (operator>>(image.getPixelsRef())) {
			image.update();
			return true;
		} else
			return false;
	}
	
//----------------------------------------
// Decoder
	void Decoder::reset() {
		this->frame = 0;
		this->data.clear();
		this->captures.clear();
		this->needPreviewUpdate = false;
	}

	void Decoder::operator<<(const ofPixels& pixels) {
		if (frame==0)
			data.allocate(pixels.getWidth(), pixels.getHeight(), payload->getWidth(), payload->getHeight());

		if (frame >= payload->getFrameCount() - 1) {
#pragma omp critical(ofLog)
			ofLogWarning("ofxGraycode") << "Can't add more frames, we've already captured a full set. please clear()";
			return;
		}

		const ofPixels* greyPixels;
		if (pixels.getNumChannels() > 1) {
			ofPixels* downsample = new ofPixels();
			downsample->allocate(pixels.getWidth(), pixels.getHeight(), OF_PIXELS_MONO);
			downsample->set(0, 0);
			const uchar* in = pixels.getPixels();
			uchar* out = downsample->getPixels();
			for (int i=0; i<pixels.size(); i++, out += (i % pixels.getNumChannels() == 0)) {
				*out += *in++ / pixels.getNumChannels();
			}
			greyPixels = downsample;
		} else
			greyPixels = &pixels;

		if (this->payload->isOffline())
			captures.push_back(*greyPixels);
		else
			payload->readPixels(frame, *greyPixels);

		frame++;

		if (frame >= payload->getFrameCount()) {
			calc();
			frame = payload->getFrameCount();
		}

		if (greyPixels != &pixels)
			delete greyPixels;
	}

	void Decoder::operator<<(ofBaseHasPixels& image) {
		this->operator<<(image.getPixelsRef());
	}

	bool Decoder::hasData() {
		return this->data.getHasData();
	}

	uint Decoder::size() const {
		return this->data.size();
	}

	const vector<ofPixels>& Decoder::getCaptures() const {
		return this->captures;
	}

	const ofPixels_<uint>& Decoder::getData() const {
		return this->data.getData();
	}

	const ofPixels& Decoder::getMean() const {
		return this->data.getMean();
	}

	void Decoder::setThreshold(uchar distanceThreshold) {
		this->data.setDistanceThreshold(distanceThreshold);
		if (this->data.getHasData())
			this->updatePreview();
	}

	uchar Decoder::getThreshold() const {
		return this->data.getDistanceThreshold();
	}

	const DataSet& Decoder::getDataSet() const {
		return this->data;
	}

	////
	//ofBaseDraws
	////
	//
	void Decoder::draw(float x,float y) {
		this->updatePreviewTextures();
		if (projectorInCamera.isAllocated())
			projectorInCamera.draw(x, y);
	}

	void Decoder::draw(float x,float y,float w, float h) {
		this->updatePreviewTextures();
		if (projectorInCamera.isAllocated())
			projectorInCamera.draw(x, y, w, h);
	}
	
	float Decoder::getHeight() {
		return projectorInCamera.getHeight();
	}

	float Decoder::getWidth() {
		return projectorInCamera.getWidth();
	}
	//
	////

	////
	//previews
	////
	//
	const ofImage& Decoder::getCameraInProjector() {
		this->updatePreviewTextures();
		return this->cameraInProjector;
	}

	const ofImage& Decoder::getProjectorInCamera() {
		this->updatePreviewTextures();
		return this->projectorInCamera;
	}
	//
	////

	////
	//file actions
	////
	//
	void Decoder::loadDataSet() {
		data.load();
		updatePreview();
	}

	void Decoder::loadDataSet(const string filename) {
		data.load(filename);
		updatePreview();
	}

	void Decoder::saveDataSet() {
		data.save();
		savePreviews();
	}

	void Decoder::saveDataSet(const string filename) {
		data.save(filename);
		savePreviews();
	}

	void Decoder::savePreviews() {
		string filename = data.getFilename();
		if (filename == "")
			filename = ofSystemSaveDialog("DataSet", "Select output path for previews").getPath();

		if (projectorInCamera.isAllocated())
			projectorInCamera.saveImage(filename + "-projectorInCamera.png");

		if (cameraInProjector.isAllocated())
			cameraInProjector.saveImage(filename + "-cameraInProjector.png");
	}
	//
	////

	void Decoder::calc() {
		ofLogNotice() << "ofxGraycode::Decoder::calc()";
		if (payload->isOffline()) {
			const PayloadOffline& payload(*(const PayloadOffline*)this->payload);
			data.calcMean(this->captures);
			payload.calc(this->captures, this->data);
		} else {
			const PayloadOnline& payload(*(const PayloadOnline*)this->payload);
			payload.calc(this->data);
		}
		updatePreview();
	}

	void Decoder::updatePreview() {
		projectorInCamera.allocate(data.getWidth(), data.getHeight(), OF_IMAGE_COLOR);
		memset(projectorInCamera.getPixels(), 0, projectorInCamera.getPixelsRef().size());

		cameraInProjector.allocate(data.getPayloadWidth(), data.getPayloadHeight(), OF_IMAGE_COLOR);
		memset(cameraInProjector.getPixels(), 0, cameraInProjector.getPixelsRef().size());

		uchar* camPix = projectorInCamera.getPixels();
		uchar* projPixels = cameraInProjector.getPixels();
		uchar* projPix;
		uint* distance = data.getDistance().getPixels();
		uint threshold = data.getDistanceThreshold();
		const uint* idx = data.getData().getPixels();
		for (int i=0; i<data.size(); i++, idx++) {
			if (*idx < payload->getSize() && *distance++ > threshold) {
				*camPix++ = 255.0f * float(*idx % payload->getWidth()) / float(payload->getWidth());
				*camPix++ = 255.0f * float(*idx / payload->getWidth()) / float(payload->getHeight());
				*camPix++ = 0.0f;

				projPix = projPixels + 3 * *idx;
				projPix[0] = 255.0f * float(i % data.getWidth()) / float(data.getWidth());
				projPix[1] = 255.0f * float(i / data.getWidth()) / float(data.getHeight());

			} else {
				memset(camPix, 0, 3);
				camPix += 3;
			}
		}

		//to be thread safe, we perform the opengl updates on next draw
		this->needPreviewUpdate = true;
	}

	void Decoder::updatePreviewTextures() {
		if (!this->needPreviewUpdate)
			return;

		projectorInCamera.update();
		cameraInProjector.update();
		this->needPreviewUpdate = false;
	}
}