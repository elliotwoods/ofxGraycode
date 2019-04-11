#include "Decoder.h"

#include "Payload/Factory.h"

namespace ofxGraycode {
	//----------
	void Decoder::clear() {
		this->frame = 0;
		this->data.clear();
		this->needPreviewUpdate = false;
		this->clearCaptures();
	}

	//----------
	void Decoder::clearCaptures() {
		this->captures.clear();
	}

	//----------
	void Decoder::operator<<(const ofPixels& pixels) {
		if (frame == 0) {
			data.allocate(pixels.getWidth(), pixels.getHeight(), payload->getWidth(), payload->getHeight());
		}

		if (frame > payload->getFrameCount() - 1) {
#pragma omp critical(ofLog)
			ofLogWarning("ofxGraycode") << "Can't add more frames, we've already captured a full set. please clear()";
			return;
		}

		if (!pixels.isAllocated()) {
			ofLogError("ofxGraycode") << "Cannot add this capture as the pixels object is empty";
			return;
		}

		const ofPixels* greyPixels;
		if (pixels.getNumChannels() > 1) {
			ofPixels* downsample = new ofPixels();
			downsample->allocate(pixels.getWidth(), pixels.getHeight(), OF_PIXELS_MONO);
			downsample->set(0, 0);
			const uint8_t* in = pixels.getData();
			uint8_t* out = downsample->getData();
			for (int i = 0; i < pixels.size(); i++, out += (i % pixels.getNumChannels() == 0)) {
				*out += *in++ / pixels.getNumChannels();
			}
			greyPixels = downsample;
		}
		else
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

	//----------
	void Decoder::operator<<(ofBaseHasPixels& image) {
		this->operator<<(image.getPixels());
	}

	//----------
	bool Decoder::hasData() const {
		return this->data.getHasData();
	}

	//----------
	uint32_t Decoder::size() const {
		return this->data.size();
	}

	//----------
	const vector<ofPixels>& Decoder::getCaptures() const {
		return this->captures;
	}

	//----------
	const ofPixels_<uint32_t>& Decoder::getData() const {
		return this->data.getData();
	}

	//----------
	const ofPixels& Decoder::getMedian() const {
		return this->data.getMedian();
	}

	//----------
	void Decoder::setThreshold(uint8_t distanceThreshold) {
		this->data.setDistanceThreshold(distanceThreshold);
		if (this->data.getHasData()) {
			this->data.calc();
			this->updatePreview();
		}
	}

	//----------
	uint8_t Decoder::getThreshold() const {
		return this->data.getDistanceThreshold();
	}

	//----------
	const DataSet& Decoder::getDataSet() const {
		return this->data;
	}

	//----------
	void Decoder::setDataSet(const DataSet& data) {
		this->data = data;
		this->updatePreview();
	}

	//----------
	void Decoder::update() {
		this->updatePreviewTextures();
	}

	//----------
	void Decoder::draw(float x, float y) const {
		if (projectorInCamera.isAllocated()) {
			projectorInCamera.draw(x, y);
		}
	}

	//----------
	void Decoder::draw(float x, float y, float w, float h) const {
		if (projectorInCamera.isAllocated()) {
			projectorInCamera.draw(x, y, w, h);
		}
	}

	//----------
	float Decoder::getWidth() const {
		return projectorInCamera.getWidth();
	}

	//----------
	float Decoder::getHeight() const {
		return projectorInCamera.getHeight();
	}

	//----------
	ofImage& Decoder::getCameraInProjector() {
		this->updatePreviewTextures();
		return this->cameraInProjector;
	}

	//----------
	ofImage& Decoder::getProjectorInCamera() {
		this->updatePreviewTextures();
		return this->projectorInCamera;
	}

	//----------
	void Decoder::loadDataSet(const string filename, Payload::Type payloadType, bool throwIfPayloadDoesntMatch) {
		//load the dataSet
		data.load(filename);

		if (data.getHasData()) {
			if (this->payload) {
				//check for payload mismatch
				if (this->payload->getWidth() != data.getPayloadWidth() || this->payload->getHeight() != data.getPayloadHeight()) {
					if (throwIfPayloadDoesntMatch) {
						throw("ofxGraycode::Decoder::loadDataSet : Payload dimensions do not match the dataSet which was just loaded.");
					}

					//if not, let's invalidate the payload
					this->payload = nullptr;
				}
			}

			auto payload = Payload::make(payloadType);
			payload->init(data.getPayloadWidth(), data.getPayloadHeight());
			this->setPayload(payload);
		}

		this->updatePreview();
		this->updatePreviewTextures();
	}

	//----------
	void Decoder::saveDataSet(const string filename) {
		data.save(filename);
		if (filename != "" && data.getFilename() != "") {
			this->savePreviews();
		}
	}

	//----------
	void Decoder::savePreviews() {
		string filename = data.getFilename();
		if (filename == "") {
			filename = ofSystemSaveDialog("DataSet", "Select output path for previews").getPath();
		}

		if (projectorInCamera.isAllocated()) {
			projectorInCamera.save(filename + "-projectorInCamera.png");
		}

		if (cameraInProjector.isAllocated()) {
			cameraInProjector.save(filename + "-cameraInProjector.png");
		}

		if (data.getMedian().isAllocated()) {
			ofImage(data.getMedian()).save(filename + "-median.png");
		}

		if (data.getMedianInverse().isAllocated()) {
			ofImage(data.getMedianInverse()).save(filename + "-medianInverse.png");
		}
	}

	//----------
	void Decoder::calc() {
		if (payload->isOffline()) {
			auto payload = static_pointer_cast<Payload::Offline>(this->payload);
			data.calcMedian(this->captures);
			payload->calc(this->captures, this->data);
		}
		else {
			auto payload = static_pointer_cast<Payload::Online>(this->payload);
			payload->calc(this->data);
		}
		updatePreview();
	}

	//----------
	void Decoder::updatePreview() {
		if (!this->payload) {
			return;
		}

		if (!this->data.getHasData()) {
			return;
		}

		projectorInCamera.allocate(data.getWidth(), data.getHeight(), OF_IMAGE_COLOR);
		memset(projectorInCamera.getPixels().getData(), 0, projectorInCamera.getPixels().size());

		cameraInProjector.allocate(data.getPayloadWidth(), data.getPayloadHeight(), OF_IMAGE_COLOR);
		memset(cameraInProjector.getPixels().getData(), 0, cameraInProjector.getPixels().size());

		uint8_t* camPix = projectorInCamera.getPixels().getData();
		uint8_t* active = (uint8_t*)data.getActive().getData();
		uint32_t threshold = (uint32_t)data.getDistanceThreshold();

		memset(camPix, 0, projectorInCamera.getPixels().size());
		const uint32_t* idx = data.getData().getData();
		for (uint32_t i = 0; i < data.size(); i++, idx++) {
			if (*idx < payload->getSize() && (*active++)) {
				*camPix++ = 255.0f * float(*idx % payload->getWidth()) / float(payload->getWidth());
				*camPix++ = 255.0f * float(*idx / payload->getWidth()) / float(payload->getHeight());
				*camPix++ = 0.0f;
			}
			else
				camPix += 3;
		}

		uint8_t* projPixels = cameraInProjector.getPixels().getData();
		uint32_t* dataInverse = this->data.getDataInverse().getData();
		float cameraWidth = this->getWidth();
		float cameraHeight = this->getHeight();
		for (uint32_t i = 0; i < this->payload->getSize(); i++, projPixels += 3, dataInverse++) {
			projPixels[0] = 255.0f * float(*dataInverse % (uint32_t)cameraWidth) / cameraWidth;
			projPixels[1] = 255.0f * float(*dataInverse / (uint32_t)cameraWidth) / cameraHeight;
		}

		//to be thread safe, we perform the opengl updates on next draw
		this->needPreviewUpdate = true;
	}

	//----------
	void Decoder::updatePreviewTextures() {
		if (!this->needPreviewUpdate)
			return;

		projectorInCamera.update();
		cameraInProjector.update();
		this->needPreviewUpdate = false;
	}
}