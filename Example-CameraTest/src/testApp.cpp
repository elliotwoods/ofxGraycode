#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowShape(1024, 512);
	payload.init(512, 512);
	encoder.init(payload);
	decoder.init(payload);
	video.initGrabber(640, 480, false);
	this->encodeFrame = true;
}

//--------------------------------------------------------------
void testApp::update(){
	video.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	message.draw(0, 0);
	decoder.draw(512, 0);

	ofDrawBitmapString("Press any key to step through encoder and decoder frames one by one", 20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if (key == 'v') {
		video.videoSettings();
		return;
	}

	if (encodeFrame) {
		if (encoder >> message)
			encodeFrame = false;
	} else {
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