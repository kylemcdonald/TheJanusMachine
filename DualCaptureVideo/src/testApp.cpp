#include "testApp.h"

void testApp::setup() {
	ofSetVerticalSync(true);
	
	kinect.init();
	ofxKinectCalibration::setClippingInCentimeters(40, 90); 
	
	kinect.open();
	
	camera.setup();
	
	capturedFrame = 0;
	recording = false;
	recordingLength = 128;
	
	cout << "allocating 128 frames of space" << endl;
	kinectBuffer.resize(1);
	kinectBuffer[0].allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	kinectBuffer.resize(recordingLength);
	
	cameraBuffer.resize(1);
	cameraBuffer[0].allocate(camera.getWidth(), camera.getHeight(), OF_IMAGE_COLOR);
	cameraBuffer.resize(recordingLength);
	cout << "done allocating" << endl;
}

void testApp::update() {
	ofBackground(100, 100, 100);
	
	camera.update();
	
	kinect.update();
	if(kinect.isFrameNew()) {
	}
	
	if(recording) {
	
		kinectBuffer[capturedFrame].setFromPixels(kinect.getDepthPixels(), kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
		cameraBuffer[capturedFrame] = camera.getPixelsRef();
	
		capturedFrame++;
		
		if(capturedFrame == recordingLength) {
			for(int i = 0; i < capturedFrame; i++) {
				ofSaveImage(kinectBuffer[i], "kinect/" + ofToString(i) + ".png");
				ofSaveImage(cameraBuffer[i], "canon/" + ofToString(i) + ".png");
			}
		
			recording = false;
		}
	}
}

void testApp::draw() {
	ofSetColor(255);
	int cw = camera.getWidth() * (640. / 1056);
	int ch = camera.getHeight() * (640. / 1056);
	camera.draw(0, 0, cw, ch);
	kinect.drawDepth(0, ch);
	
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
		recording = true;
	}
}