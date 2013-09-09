#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	this->payload.init(1024, 768);
	this->process();

	gui.init();
	gui.add(mean, "mean");
	gui.add(graycode, "graycode");
	gui.add(uv, "uv");
	gui.add(graycodeToBinary, "graycodeToBinary");
}

//--------------------------------------------------------------
void testApp::process() {

	clock_t begin = clock();

	//string path = ofSystemLoadDialog("Select folder of graycode captures", true).getPath();
	string path = "c:\\dev\\openFrameworks\\Research.ReprojectionScanning\\data\\scans\\head";
	ofDirectory directory;
	directory.listDir(path);
	directory.sort();

	const auto files = directory.getFiles();
	const int imageCount = files.size();
	images.clear();
	images.reserve(imageCount);

	for (auto & file : files) {
		ofLogNotice() << "Loading image " << file.getFileName();
		images.push_back(ofImage());
		images.back().loadImage(file);
	}

	auto end = clock();
	ofLogNotice() << "It took " << (float) (end - begin) * 1000.0f / (float) (CLOCKS_PER_SEC) << "ms to load images";
	begin = clock();

	ofImage & referenceImage(images[0]);
	ofFbo::Settings settings;
	settings.width = referenceImage.getWidth();
	settings.height = referenceImage.getHeight();

	// Currently we allocate all with 32f due to awful support 
	//  in openFrameworks for alternate GL formats in fbo's
	settings.internalformat = GL_RGB32F;
	mean.allocate(settings);
	graycode.allocate(settings);
	uv.allocate(settings);

	end = clock();
	ofLogNotice() << "It took " << (float) (end - begin) * 1000.0f / (float) (CLOCKS_PER_SEC) << "ms to allocate surfaces";
	begin = clock();

	addBits.load("passThrough.vert", "addBits.frag");
	decodeUV.load("passThrough.vert", "decodeUV.frag");

	auto endLoadShaders = clock();
	end = clock();
	ofLogNotice() << "It took " << (float) (end - begin) * 1000.0f / (float) (CLOCKS_PER_SEC) << "ms to load shader files";
	begin = clock();

	//--
	//setup graycode to binary lookup table
	//
	ofShortPixels graycodeToBinaryPixels;
	graycodeToBinaryPixels.allocate(4096, 1, OF_IMAGE_GRAYSCALE);
	for (int i=0; i<graycodeToBinaryPixels.getWidth(); i++) {
		graycodeToBinaryPixels[this->payload.toGraycode(i)] = i;
	}
	graycodeToBinary.allocate(graycodeToBinaryPixels.getWidth(), 1, GL_LUMINANCE16);
	graycodeToBinary.loadData(graycodeToBinaryPixels);
	auto data = graycodeToBinary.getTextureData();
	cout << data.glType;
	//
	//--

	end = clock();
	ofLogNotice() << "It took " << (float) (end - begin) * 1000.0f / (float) (CLOCKS_PER_SEC) << "ms to load graycode into memory";
	begin = clock();

	//--
	//calc mean
	//
	ofPushStyle();
	mean.begin();
	ofClear(0);

	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f / (float) imageCount);
	for(auto & image : images) {
		image.draw(0,0);
	}

	ofDisableBlendMode();

	mean.end();
	ofPopStyle();
	//
	//--

	end = clock();
	ofLogNotice() << "It took " << (float) (end - begin) * 1000.0f / (float) (CLOCKS_PER_SEC) << "ms to calc mean";
	begin = clock();


	//--
	//set bit pattern
	//
	ofPushStyle();
	graycode.begin();
	ofClear(0);
	addBits.begin();
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	addBits.setUniformTexture("mean", mean, 1);
	addBits.setUniform1f("threshold", 5.0f / 255.0f);

	int bit = 0;
	for(auto & image : images) {
		addBits.setUniform1i("axis", bit > this->payload.getFrameCountX());
		addBits.setUniform1i("bit", bit++ % this->payload.getFrameCountX());
		image.draw(0,0);
	}

	ofDisableBlendMode();
	addBits.end();
	graycode.end();
	ofPopStyle();
	//
	//--

	end = clock();
	ofLogNotice() << "It took " << (float) (end - begin) * 1000.0f / (float) (CLOCKS_PER_SEC) << "ms to accumulate bits";
	begin = clock();

	//--
	//decode to uv
	//
	uv.begin();
	decodeUV.begin();
	decodeUV.setUniformTexture("graycodeToBinary", graycodeToBinary, 1);
	graycode.draw(0,0);
	decodeUV.end();
	uv.end();
	//
	//--

	end = clock();
	ofLogNotice() << "It took " << (float) (end - begin) * 1000.0f / (float) (CLOCKS_PER_SEC) << "ms to decode to uv";
	begin = clock();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == ' ')
		process();
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