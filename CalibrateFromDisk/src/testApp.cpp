#include "testApp.h"

void calibrate(Calibration& calib, string dir) {
	ofDirectoryLister dirList;
	ofImage cur;
	
	dirList.listDir(dir);
	calib.setBoardSize(10, 7);
	calib.setSquareSize(1);
	int total = 0;
	for(int i = 0; i < dirList.size(); i++) {
		cur.loadImage(dirList.getPath(i));
		if(calib.add(cur)) {
			total++;
		}
	}
	calib.calibrate();
	
	cout << "calibrated with " << total << "/" << dirList.size() << endl;
}

void testApp::setup() {	
	Calibration calib;
	calibrate(calib, "images/");
	calib.save("calibration.yml");
	
	cout << "reprojection error: " << endl << calib.getReprojectionError() << endl;
	cout << "distortion coefficients: " << endl << calib.getDistortionCoefficients() << endl;
	cout << "camera matrix: " << endl << calib.getCameraMatrix() << endl;
}

void testApp::update() {
}

void testApp::draw() {
}
