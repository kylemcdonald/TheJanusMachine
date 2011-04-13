#include "testApp.h"

void calibrate(Calibration& calib, string dir) {
	calib.setBoardSize(10, 7);
	calib.setSquareSize(1);
	calib.calibrateFromDirectory(dir);
}

void testApp::setup() {	
	Calibration calib;
	calibrate(calib, "images/");
	calib.save("calibration.yml");
	
	cout << "reprojection error: " << endl << calib.getReprojectionError() << endl;
	cout << "camera matrix: " << endl << calib.getDistortedIntrinsics().getCameraMatrix() << endl;
}

void testApp::update() {
}

void testApp::draw() {
}
