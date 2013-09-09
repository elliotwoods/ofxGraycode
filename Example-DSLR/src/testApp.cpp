#include "testApp.h"

void testApp::setup() {
    ofBackground(200, 100, 100);
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
    
    if (!camera.setup()) {
        ofSystemAlertDialog("No Canon DSLR found. Please connect camera before starting application.");
        ofExit();
    }
    
    this->doSend = false;
    this->isScanning = false;
}

void testApp::update() {
    if (camera.isPhotoNew()) {
        decoder << camera.getPhotoPixels();
        doSend = true;
    }
    if (this->isScanning && !decoder.hasData()) {
        ofHideCursor();
    } else {
        ofShowCursor();
    }
}

void testApp::draw() {
    if (this->isScanning) {
        message.draw(0, 0);
        
        if (decoder.hasData()){
            decoder.draw(0, 0, ofGetWidth(), ofGetHeight());
            
            ofDrawBitmapStringHighlight("Press [s] to save dataset", 30, 30);
            ofDrawBitmapStringHighlight("Press [UP]/[DOWN] to change threshold (" + ofToString( (int) decoder.getThreshold()) + ")", 30, 60);

        } else if (doSend) {
            if (encoder >> message) {
                camera.takePhoto();
                doSend = false;
            } else {
                decoder.update();
            }
        }
    } else {
        ofDrawBitmapStringHighlight("Press [SPACE] to start scanning", 30, 30);
        ofDrawBitmapStringHighlight("Press [f] to toggle fullscreen", 30, 60);
        ofDrawBitmapStringHighlight("When scanning is complete, please select a location to save results", 30, 90);
        ofDrawBitmapStringHighlight("Press [SPACE] whilst scanning to stop", 30, 120);
        
        stringstream ss;
        ss << "Current resolution : " << ofGetWidth() << "x" << ofGetHeight() << ". Takes " << ceil(log2(ofGetWidth())) + ceil(log2(ofGetHeight()))  << " frames.";
        ofDrawBitmapStringHighlight(ss.str(), 30, 150);
    }
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
        if (!this->isScanning) {
			//start scanning
            payload.init(ofGetWidth(), ofGetHeight());
            encoder.init(payload);
            decoder.init(payload);
            this->doSend = true;
            this->isScanning = true;
        } else {
			//stop scanning
            this->isScanning = false;
        }
	}
    
    if (key == 'f')
        ofToggleFullscreen();
    
    if (decoder.hasData()) {
        if (key == 's') {
            decoder.saveDataSet();
        }
        if (key == OF_KEY_UP) {
            int threshold = decoder.getThreshold() + 1;
            threshold = ofClamp(threshold, 0, 255);
            decoder.setThreshold(threshold);
        }
        if (key == OF_KEY_DOWN) {
            int threshold = decoder.getThreshold() - 1;
            threshold = ofClamp(threshold, 0, 255);
            decoder.setThreshold(threshold);
        }
    }
}