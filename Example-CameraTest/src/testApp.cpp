#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowShape(1024, 512);
	payload.init(512, 512);
	encoder.init(payload);
	decoder.init(payload);
	video.listDevices();
	video.setDeviceID(1);
	video.initGrabber(1280, 720, true);
	this->encodeFrame = true;
	this->autoAdvance = false;
}

//--------------------------------------------------------------
void testApp::update(){
	video.update();

	if (autoAdvance && decoder.hasData() == false) {
		if (!encodeFrame) {
			int time = ofGetElapsedTimeMillis();
			while (ofGetElapsedTimeMillis() < time + WAIT_TIME) {
				ofSleepMillis(1);
				video.update();
			}
		}
		advance();
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	if (message.isAllocated())
		message.draw(0, 0);

	if (decoder.getProjectorInCamera().isAllocated())
		decoder.draw(512, 0, 512, 512);
	else
		video.draw(512, 0, 512, 512);

	ofDrawBitmapString("Press any key to step through encoder and decoder frames one by one", 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if (key == 'v') {
		video.videoSettings();
		return;
	}
	
	if (key == 'r') {
		reset();
		return;
	}
	
	if (key == 's') {
		decoder.saveDataSet();
	}
	
	if (key == 'a') {
		autoAdvance = true;
		cout << "Auto advance" << endl;
	}


	advance();
}

//--------------------------------------------------------------
void testApp::reset() {
	decoder.reset();
	encoder.reset();
	autoAdvance = false;
	encodeFrame = true;
}

//--------------------------------------------------------------
void testApp::advance() {
	if (encodeFrame) {
		cout << "encode" << endl;
		if (encoder >> message)
			encodeFrame = false;
	} else {
		cout << "decode" << endl;
		decoder << video;
		encodeFrame = true;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}