#include "testApp.h"

void testApp::setup() {	
	ofDirectoryLister dirList;
	Calibration calib;
	ofImage cur;
	
	dirList.listDir("images/");
	
	calib.setBoardSize(6, 9);
	calib.setSquareSize(1);
	
	for(int i = 0; i < dirList.size(); i++) {
		cur.loadImage(dirList.getPath(i));
		calib.add(cur);
	}
	calib.calibrate();
	calib.save("calibration.yml");
	
	cout << "reprojection error: " << endl << calib.getReprojectionError() << endl;
	cout << "distortion coefficients: " << endl << calib.getDistortionCoefficients() << endl;
	cout << "camera matrix: " << endl << calib.getCameraMatrix() << endl;
}

void testApp::update() {
}

void testApp::draw() {
}
