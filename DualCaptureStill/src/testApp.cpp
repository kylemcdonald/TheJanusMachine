#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	
	kinect.init(true);
	
	kinect.open();
	
	camera.setup();
	
	capturedFrame = 0;
}

void testApp::update() {
	ofBackground(100, 100, 100);
	
	camera.update();
	
	kinect.update();
	if(kinect.isFrameNew()) {
	}
}

void testApp::draw() {
	ofSetColor(255);
	int cw = camera.getWidth() * (640. / 1056);
	int ch = camera.getHeight() * (640. / 1056);
	camera.draw(0, 0, cw, ch);
	kinect.draw(0, ch);
	
	ofSetColor(ofColor::red);
	ofLine(320, 0, 320, ofGetHeight());
	ofLine(0, ch / 2, ofGetWidth(), ch / 2);
	ofLine(0, ch + 240, ofGetWidth(), ch + 240);
}

void testApp::exit() {
	kinect.close();
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		ofSaveImage(camera.getPixelsRef(), "canon-" + ofToString(capturedFrame) + ".png");
		
		ofPixels pix;
		pix.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
		pix.setFromPixels(kinect.getPixels(), kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
		ofSaveImage(pix, "kinect-" + ofToString(capturedFrame) + ".png");
		capturedFrame++;
	}
}