#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	int y = 10;
	ofDrawBitmapString("Press any key to load a file and output as correspondance table file + cam/proj properties at the same path", 10, y+=10);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	data.load();
	string filename = ofFilePath::removeExt(data.getFilename());

	ofstream file((filename + "-correspondances.txt").c_str());
	file << "Camera pixel\tProjector pixel\tDistance (score)";
	const unsigned char* active = data.getActive().getPixels();
	const uint* input = data.getData().getPixels();
	const uint* distance = data.getDistance().getPixels();
	for (uint i=0; i<data.size(); i++, active++, input++, distance++) {
		if (*active)
			file << i << "\t" << *input << "\t" << *distance << endl;
	}
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