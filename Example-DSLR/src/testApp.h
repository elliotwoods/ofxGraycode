#pragma once

#include "ofMain.h"

#include "ofxEdsdk.h"
#include "ofxGraycode.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
    
	ofxEdsdk::Camera camera;
    
    ofImage message;
    
    ofxGraycode::PayloadGraycode payload;
    ofxGraycode::Encoder encoder;
    ofxGraycode::Decoder decoder;
    
    bool doSend;
    bool isScanning;
    
    void sendNext();
};
