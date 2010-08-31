#include "ConnexionCamera.h"

ConnexionCamera::ConnexionCamera() :
	curZoom(1600),
	minZoom(400),
	maxZoom(3200),
	zoomSpeed(1),
	rotationSpeed(.001),
	rotationMomentum(.9) {
}

void ConnexionCamera::draw() {
	ConnexionData& data = ofxConnexion::connexionData;
	
	float zoomVelocity = -data.translation[1] * zoomSpeed;
	curZoom += zoomVelocity;
	curZoom = ofClamp(curZoom, minZoom, maxZoom);
	ofxVec3f& avg = Particle::avg;
	gluLookAt(0, 0, curZoom,
						avg.x, avg.y, avg.z,
						0, 1, 0);
	cout << data.toString() << endl;
	// (rotation speed should technically be affected by the fps)
	ofxQuaternion curOrientationVelocity;
	curOrientationVelocity.makeRotate(-data.rotation[0] * rotationSpeed, xunit3f,
																		+data.rotation[2] * rotationSpeed, yunit3f,
																		+data.rotation[1] * rotationSpeed, zunit3f);
	
	lastOrientationVelocity.slerp(rotationMomentum, curOrientationVelocity, lastOrientationVelocity);
	
	curOrientation *= lastOrientationVelocity;
	float amount;
	ofxVec3f angle;
	curOrientation.getRotate(amount, angle);
	glRotatef(ofRadToDeg(amount), angle.x, angle.y, angle.z);
}