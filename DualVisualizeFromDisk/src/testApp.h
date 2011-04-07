#pragma once

#include "ofMain.h"

#include "ofxCv.h"
using namespace cv;
using namespace ofxCv;

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	
	Calibration leftCalibration, rightCalibration;
	Mat rotation, translation;
	ofEasyCam cam;
	
	int curImage;
};
