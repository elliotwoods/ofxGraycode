#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(50,50,50);

	//start up the image
	adjust(256,256);
	
	volume = ofVec3f(4, 2, 2);
	mean = ofVec3f(0, 0, 0);
	
	count = 0;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	camera.begin();
	
	ofPushStyle();
	ofSetColor(250,150,150);
	ofDrawGrid();
	ofPopStyle();
	mesh.drawVertices();
	camera.end();
	
	int y = 0;
	int width = img.getWidth();
	int height = img.getHeight();
	ofDrawBitmapString("Image size: " + ofToString(width) + "," + ofToString(height), 10, y+=15);
	ofDrawBitmapString("Volume: " + ofToString(volume), 10, y+=15);
	ofDrawBitmapString("Target mean position: " + ofToString(mean), 10, y+=15);
	ofDrawBitmapString("Vertex count: " + ofToString(count), 10, y+=15);
	y+=15;
	ofDrawBitmapString("[SPACE] = try add 1000 vertices", 10, y+=15);
	ofDrawBitmapString("[c] = toggle camera cursor", 10, y+=15);
	ofDrawBitmapString("[f] = toggle fullscreen", 10, y+=15);
	ofDrawBitmapString("[s] = take a screenshot", 10, y+=15);
	ofDrawBitmapString("[d] = save sparse set", 10, y+=15);
	ofDrawBitmapString("[LEFT/RIGHT] = adjust width -/+256", 10, y+=15);
	ofDrawBitmapString("[UP/DOWN] = adjust height -/+256", 10, y+=15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == ' ')
		generate();
	if (key == 'c')
		camera.toggleCursorDraw();
	if (key == 'f')
		ofToggleFullscreen();
	if (key == 's') {
		ofImage screenshot;
		screenshot.grabScreen(0,0,ofGetWidth(),ofGetHeight());
		screenshot.saveImage(ofSystemLoadDialog("Save screenshot...").fileName);
	}
	if (key == 'd')
		save();
	
	if (key == OF_KEY_LEFT)
		adjust(-256, 0);
	if (key == OF_KEY_RIGHT)
		adjust(+256, 0);
	if (key == OF_KEY_UP)
		adjust(0, -256);
	if (key == OF_KEY_DOWN)
		adjust(0, +256);
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

//--------------------------------------------------------------
void testApp::generate(){ 
	if (!img.isAllocated())
		ofLogWarning() << "increase the size of your image before generating";
	else {
		int idx;
		ofVec3f* xyz;
		ofVec3f halfVolume = volume / 2.0f;
		for (int i=0; i<1000; i++) {
			idx = ofRandom(img.getPixelsRef().size());
			xyz = (ofVec3f*)(img.getPixels() + idx);
			
			//check it's not already taken
			if (xyz->lengthSquared() == 0.0f) {
				xyz->x = ofRandom(-halfVolume.x, halfVolume.x) + mean.x;
				xyz->y = ofRandom(-halfVolume.y, halfVolume.y) + mean.y;
				xyz->z = ofRandom(-halfVolume.z, halfVolume.z) + mean.z;
				mesh.addVertex(*xyz);
				count++;
			}
		}
		img.update();
	}
}

//--------------------------------------------------------------
void testApp::adjust(int dx, int dy) {
	int newWidth = img.getWidth() + dx;
	int newHeight = img.getHeight() + dy;
	
	if (newWidth <= 0 || newHeight <= 0)
		ofLogWarning() << "Cannot reduce size to <=0";
	else {
		img.clear();
		mesh.clearVertices();
		img.allocate(newWidth, newHeight, OF_IMAGE_COLOR);
		count = 0;
	}
}

//--------------------------------------------------------------
void testApp::save() {
	img.saveImage(ofSystemSaveDialog("sparse.pfm", "Save sparse image...").fileName);
}