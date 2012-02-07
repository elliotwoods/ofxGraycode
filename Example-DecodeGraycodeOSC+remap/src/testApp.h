#pragma once

#include "ofMain.h"
#include "ofxGrabCam.h"
#include "ofxGraycode.h"
#include "ofxOsc.h"
#include "ofxCv.h"

#define SERVER_PORT 5588

using namespace ofxGraycode;
using namespace ofxCv;
using namespace cv;

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void processInput();
	void moveFrame(int distance);
	void moveThreshold(int distance);

	Mat buildRemap();

	PayloadGraycode payload;
	ofVideoGrabber video;
	Decoder decoder;

	int previewFrame;
	ofImage capturePreview;
	ofxOscReceiver rx;
};
