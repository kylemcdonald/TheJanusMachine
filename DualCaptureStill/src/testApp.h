#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxEdsdk.h"

class testApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
	
	ofxKinect kinect;
	ofxEdsdk::Camera camera;
	
	int capturedFrame;
};
