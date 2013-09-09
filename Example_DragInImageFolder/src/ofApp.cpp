#include "ofApp.h"
#include <locale>

//--------------------------------------------------------------
void ofApp::setup(){
	this->payload.init(1920,1080);
	this->decoder.init(payload);
	this->showProjector = false;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	if (this->decoder.hasData()) {
		if (this->showProjector) {
			this->decoder.getCameraInProjector().draw(0,0,ofGetWidth(),ofGetHeight());
		} else {
			this->decoder.getProjectorInCamera().draw(0,0,ofGetWidth(),ofGetHeight());
		}
	}

	ofDrawBitmapStringHighlight("[s] = save", 20, 30);
	ofDrawBitmapStringHighlight("[c] = save", 20, 60);
	ofDrawBitmapStringHighlight(string("Thresold = ") + ofToString(this->decoder.getThreshold()) + "...[up]/[down] to change", 20, 90);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
	case OF_KEY_DOWN:
		{
			int threshold = this->decoder.getThreshold();
			threshold--;
			if (threshold >=0) {
				this->decoder.setThreshold(threshold);
			}
		}
		break;
	case OF_KEY_UP:
		{
			int threshold = this->decoder.getThreshold();
			threshold++;
			if (threshold <=255) {
				this->decoder.setThreshold(threshold);
			}
		}
		break;
	case 's':
		this->decoder.saveDataSet();
		break;
	case 'c':
		this->decoder.reset();
		break;
	}
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
	vector<string> filenamesDragged;
	for(auto entry : dragInfo.files) {
		if (ofFile(entry).isDirectory()) {
			ofDirectory dir;
			dir.listDir(entry);
			dir.sort();
			for(auto file : dir.getFiles()) {
				filenamesDragged.push_back(file.getFileName());
			}
		} else {
			filenamesDragged.push_back(entry);
		}
	}

	for(auto file : filenamesDragged) {
		ofImage image;
		image.loadImage(file);
		ofLogNotice() << "Current frame: " << decoder.getFrame();
		ofLogNotice() << "Loading image: " << file;
		decoder << image;
	}
}