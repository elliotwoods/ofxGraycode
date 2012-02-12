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

	void BaseCodec::init(Payload& payload) {
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
	}

	void Decoder::operator<<(const ofPixels& pixels) {
		if (frame==0)
			data.allocate(pixels.getWidth(), pixels.getHeight(), payload->getWidth(), payload->getHeight());

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

 		if (++frame >= payload->getFrameCount()) {
			calc();
			frame--;
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
		preview.draw(x, y);
	}

	void Decoder::draw(float x,float y,float w, float h) {
		preview.draw(x, y, w, h);
	}
	
	float Decoder::getHeight() {
		return preview.getHeight();
	}

	float Decoder::getWidth() {
		return preview.getWidth();
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
	}

	void Decoder::saveDataSet(const string filename) {
		data.save(filename);
	}
	//
	////

	void Decoder::calc() {
		ofLogNotice() << "ofxGraycode::Decoder::calc()";
		if (payload->isOffline()) {
			PayloadOffline& payload(*(PayloadOffline*)this->payload);

			data.calcMean(this->captures);
			payload.calc(this->captures, this->data);
		} else {
			PayloadOnline& payload(*(PayloadOnline*)this->payload);
			payload.calc(this->data);
		}
		updatePreview();
	}

	void Decoder::updatePreview() {
		preview.allocate(data.getWidth(), data.getHeight(), OF_IMAGE_COLOR);
		preview.getPixelsRef().set(0,0);
		preview.getPixelsRef().set(1,0);
		preview.getPixelsRef().set(2,0);
		
		uchar* pix = preview.getPixels();
		uint* distance = data.getDistance().getPixels();
		uint threshold = data.getDistanceThreshold();
		const uint* idx = data.getData().getPixels();
		for (int i=0; i<data.size(); i++, idx++) {
			if (*idx < payload->getSize() && *distance++ > threshold) {
				*pix++ = 255.0f * float(*idx % payload->getWidth()) / float(payload->getWidth());
				*pix++ = 255.0f * float(*idx / payload->getWidth()) / float(payload->getHeight());
				*pix++ = 0.0f;
			} else {
				memset(pix, 0, 3);
				pix += 3;
			}
		}
		preview.update();
	}
}