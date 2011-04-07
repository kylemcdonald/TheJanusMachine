#include "testApp.h"

void testApp::setup() {	
	dirList.listDir("images/");
	i = 0;
	lasti = 0;
	
	calib.load("calibration.yml");
			
	cout << "reprojection error: " << endl << calib.getReprojectionError() << endl;
	cout << "distortion coefficients: " << endl << calib.getDistortionCoefficients() << endl;
	cout << "camera matrix: " << endl << calib.getCameraMatrix() << endl;
	cout << "fov: " << endl << calib.getFov() << endl;
	cout << "sensor size: " << endl << calib.getSensorSize() << endl;
	cout << "focal length: " << endl << calib.getFocalLength() << endl;
	
	updateImage();
}

void testApp::update() {
	if(i != lasti) {
		updateImage();
	}
	lasti = i;
}

void testApp::updateImage() {
	cur.loadImage(dirList.getPath(i));
	cur.update();

	undistorted.loadImage(dirList.getPath(i));
	calib.undistort(undistorted);
	undistorted.update();
}

void testApp::draw() {
	ofEnableAlphaBlending();
	ofSetColor(255);
	cur.draw(0, 0);
	ofSetColor(255);
	undistorted.draw(0, 480);
}

void testApp::keyPressed(int key) {
	if(key == OF_KEY_UP) {
		i--;
	} else if(key == OF_KEY_DOWN) {
		i++;
	}
	if(i < 0) {
		i = 0;
	} else if(i >= dirList.size()) {
		i = dirList.size() - 1;
	}
}