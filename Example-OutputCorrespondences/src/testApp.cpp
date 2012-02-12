#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowShape(1024,30);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	int y = 10;
	ofDrawBitmapString("Press any key to load a file and output as correspondence table file + cam/proj properties at the same path", 10, y+=10);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	data.load();
	string filename = data.getFilename();

	ofstream file((filename + "-correspondences.txt").c_str());
	file << "Camera pixel (index, x, y)\tProjector pixel (index, x, y)\tDistance (score)" << endl;
	
	const unsigned char* active = data.getActive().getPixels();
	const uint* input = data.getData().getPixels();
	const uint* distance = data.getDistance().getPixels();
	
	ofVec2f camxy, projxy;
	int camWidth = data.getWidth();
	int camHeight = data.getHeight();
	int projWidth = data.getPayloadWidth();
	int projHeight = data.getPayloadHeight();

	for (uint i=0; i<data.size(); i++, active++, input++, distance++) {
		if (*active) {
			camxy = getNorm(i, camWidth, camHeight);
			projxy = getNorm(i, projWidth, projHeight);
			file << i << "\t" << camxy.x << "\t" << camxy.y << "\t" <<
			*input << "\t" << projxy.x << "\t" << projxy.y << "\t" << *distance << endl;
		}

		if (i % 1000 == 0)
			cout << ((float)i / (float)data.size() * 100.0f) << "% complete" << endl;
	}
	cout << "done." << endl;
	file.close();
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
inline ofVec2f testApp::getNorm(const uint index, const uint width, const uint height) {
	uint x = index % width;
	uint y = index / width;
	return ofVec2f(2.0f * (float(x) + 0.5) / float(width) - 1.0f,
					1.0f - 2.0f * (float(y) + 0.5) / float(height));
}