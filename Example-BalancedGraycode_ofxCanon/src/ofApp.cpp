#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	this->gui.init();

	this->payload = make_shared<ofxGraycode::Payload::BalancedGraycode>();
	this->payload->init(1920, 1080);
	this->encoder.init(this->payload);
	this->decoder.init(this->payload);

	auto devices = ofxCanon::listDevices();
	if (devices.empty()) {
		ofSystemAlertDialog("Please attach a Canon camera and try again");
		ofExit();
	}

	this->device = devices.front();
	this->device->open();

	auto strip = this->gui.addStrip();
	{
		strip->setCellSizes({ -1, 300 });

		{
			auto grid = ofxCvGui::Panels::Groups::makeGrid();
			grid->add(ofxCvGui::Panels::makeImage(this->message, "Message"));
			grid->add(ofxCvGui::Panels::makeImage(this->photo, "Photo"));
			grid->add(ofxCvGui::Panels::makeImage(this->decoder.getProjectorInCamera(), "Projector in camera"));
			grid->add(ofxCvGui::Panels::makeImage(this->decoder.getCameraInProjector(), "Camera in projector"));

			strip->add(grid);
		}

		{
			auto widgets = ofxCvGui::Panels::makeWidgets();
			strip->add(widgets);

			widgets->addFps();
			widgets->addMemoryUsage();

			widgets->addButton("Render output files", [this]() {
				this->encoder.init(this->payload);
				int index = 0;
				while (this->encoder >> this->message) {
					index++;
					auto indexString = ofToString(index);
					while (indexString.size() < 3) {
						indexString = "0" + indexString;
					}
					ofSaveImage(this->message, indexString + ".png");
				}
			});

			widgets->addButton("Take photo", [this]() {
				this->takePhoto();
			}, ' ');

			widgets->addLiveValue<int>("Decoder frame", [this]() {
				return this->decoder.getFrame();
			});

			widgets->addButton("Save...", [this]() {
				this->decoder.saveDataSet();
			});

			widgets->addButton("Load...", [this]() {
				this->decoder.loadDataSet();
			});

			widgets->addEditableValue<int>("Threshold [/255]", [this]() {
				return this->decoder.getThreshold();
			}, [this](string valueString) {
				this->decoder.setThreshold(ofToInt(valueString));
			});
		}
	}
	

}

//--------------------------------------------------------------
void ofApp::update() {
	this->decoder.update();
	this->device->update();
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::takePhoto() {
	auto photoResult = this->device->takePhoto();
	if (photoResult) {
		ofLoadImage(this->photo, *photoResult.encodedBuffer);
		this->photo.update();
		this->decoder << this->photo;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
