#pragma once

#include "ofMain.h"

#include "ofxCv.h"
using namespace cv;
using namespace ofxCv;

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void updateImage();
	void draw();
	void keyPressed(int key);
	
	ofDirectoryLister dirList;
	Calibration calib;
	ofImage cur, undistorted;
	int lasti, i;
};
