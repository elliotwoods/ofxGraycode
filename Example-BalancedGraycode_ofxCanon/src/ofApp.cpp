#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	this->payload = make_shared<ofxGraycode::Payload::BalancedGraycode>();
	this->payload->init(1920, 1080);
	this->encoder.init(this->payload);
	this->decoder.init(this->payload);

	ofPixels message;
	uint32_t index = 0;
	while (this->encoder >> message) {
		index++;
		this->encoder >> message;
		cout << "Frame " << index << endl;
		this->decoder << message;
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	this->decoder.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	this->decoder.draw(0, 0);
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
