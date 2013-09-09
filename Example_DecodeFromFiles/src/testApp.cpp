#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	payload.init(1920, 1080);
	decoder.init(payload);
	
	string path = ofSystemLoadDialog("Select folder of images", true, ofToDataPath("")).getPath();
	ofLogNotice() << "Adding images from " << path;
	
	ofDirectory folder;
	folder.listDir(path);
	folder.sort();
	
	auto files = folder.getFiles();
	
	for (const auto it : files) {
		ofImage image(it);
		image.update();
		if (image.isAllocated()) {
			ofLogNotice() << "Adding file " << it.getFileName();
			decoder << image;
		}
	}
	
	this->state = StateViewCamera;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	switch (state) {
		case StateViewCamera:
			decoder.draw(0, 0, ofGetWidth(), ofGetHeight());
			break;
		case StateViewProjector:
			decoder.getCameraInProjector().draw(0, 0);
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch (key) {
		case ' ':
			state++;
			if (state == StateEnd) {
				state = StateViewCamera;
			}
			break;
			
		case OF_KEY_UP:
			decoder.setThreshold(decoder.getThreshold() + 5);
			break;
		case OF_KEY_DOWN:
			decoder.setThreshold(decoder.getThreshold() - 5);
			break;
			
		case 's':
			decoder.saveDataSet();
			break;
		case 'l':
			decoder.loadDataSet();
			break;
			
		default:
			break;
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