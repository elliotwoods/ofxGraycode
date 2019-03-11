#pragma once

#include "ofMain.h"
#include "ofxCvGui.h"
#include "ofxGraycode.h"
#include "ofxCanon.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void takePhoto();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofxCvGui::Builder gui;

		shared_ptr<ofxGraycode::Payload::BalancedGraycode> payload;
		ofxGraycode::Encoder encoder;
		ofxGraycode::Decoder decoder;
		ofParameter<float> threshold;

		ofImage message;
		ofImage photo;

		shared_ptr<ofxCanon::Device> device;
};
