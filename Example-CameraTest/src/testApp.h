#pragma once

#include "ofMain.h"
#include "ofxGraycode.h"

//wait for a certain amount of time to allow the camera to capture the encoded frame
#define WAIT_TIME 300

using namespace ofxGraycode;

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	
	void advance();
	void reset();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	PayloadGraycode payload;
	Encoder encoder;
	Decoder decoder;
	ofImage message;
	ofVideoGrabber video;

	bool encodeFrame;
	bool autoAdvance;
};
