#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetWindowShape(1024, 1024);
	ofSetLogLevel(OF_LOG_NOTICE);

	payload.init(512, 512);
	video.initGrabber(640, 480, true);
	decoder.init(payload);

	rx.setup(SERVER_PORT);

	previewFrame = -2;
}

//--------------------------------------------------------------
void testApp::update(){
	video.update();

	while (rx.hasWaitingMessages()) 
		processInput();
}

//--------------------------------------------------------------
void testApp::draw(){
	video.draw(0, 0, 512, 512);
	if (decoder.hasData() && previewFrame == -2)
		decoder.draw(512, 0, 512, 512);
	else if (capturePreview.isAllocated())
		capturePreview.draw(512, 0, 512, 512);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == OF_KEY_LEFT)
		moveFrame(-1);
	if (key == OF_KEY_RIGHT)
		moveFrame(+1);
	if (key == OF_KEY_UP)
		moveThreshold(+5);
	if (key == OF_KEY_DOWN)
		moveThreshold(-5);
	if (key == 'v')
		video.videoSettings();
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
void testApp::processInput(){ 
	ofxOscMessage msg;
	rx.getNextMessage(&msg);

	if (msg.getAddress() == "/capture") {
		ofLogNotice() << "capture frame " << decoder.getFrame();
		decoder << video;
	}

	if (msg.getAddress() == "/clear") {
		ofLogNotice() << "clear frames";
		decoder.reset();
	}
}

//--------------------------------------------------------------
void testApp::moveFrame(int distance){ 
	previewFrame = ofClamp(previewFrame + distance, -2, decoder.getCaptures().size()-1);

	ofLogNotice() << "Preview frame #" << previewFrame;
	if (previewFrame >= 0 && previewFrame < decoder.getCaptures().size())
		capturePreview = ofImage(decoder.getCaptures()[previewFrame]);
	if (previewFrame == -1)
		capturePreview = ofImage(decoder.getMean());
}

//--------------------------------------------------------------
void testApp::moveThreshold(int distance){ 
	decoder.setThreshold(ofClamp((int)decoder.getThreshold() + distance, 0, 255));
	ofLogNotice() << "Threshold set at " << (int)decoder.getThreshold();
}

//--------------------------------------------------------------
// mirrors kyle's https://github.com/elliotwoods/ProCamToolkit/blob/master/SharedCode/ofxProCamToolkit.cpp
Mat testApp::buildRemap() {
	int tw = decoder.getWidth();
	int th = decoder.getHeight();
	int sw = payload.getWidth();
	int sh = payload.getHeight();

	Mat remap(tw, th, CV_32FC2);
	Mat total(tw, th, CV_32FC2);
	for (int j=0; j<sh; j++)
		for (int i=0; i<sw; i++) {

		}
	divide(remap, total, remap);
	return remap;
}