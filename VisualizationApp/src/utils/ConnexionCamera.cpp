#include "ConnexionCamera.h"


// this is for update;
float rotationDistance(ofxQuaternion& from, ofxQuaternion& to) {
	ofxVec3f xunit3f(1, 0, 0);
	ofxVec3f fromVec = from * xunit3f;
	ofxVec3f toVec = to * xunit3f;
	return acosf(fromVec.dot(toVec));
}


ConnexionCamera::ConnexionCamera() :
	curZoom(1600),
	minZoom(400),
	maxZoom(3200),
	zoomSpeed(1),
	rotationSpeed(.001),
	rotationMomentum(.9) {
		
		lastFrameZoom = 0;
		
	}

void ConnexionCamera::addRotation(ofxQuaternion rotation) {
	lastOrientationVelocity *= rotation;
}


void ConnexionCamera::update(){
	
	
	if (ofGetFrameNum() > 5){
		zoomChangeAmount= fabs(curZoom - lastFrameZoom);
		quaternionChangeAmount = rotationDistance(curOrientation, lastOrientation);
		//cout << "camera rotatione amount " << quaternionChangeAmount << endl;
	}
	
	lastOrientation = curOrientation;
	lastFrameZoom = curZoom;
}



void ConnexionCamera::draw(float mouseX, float mouseY) {
	ConnexionData& data = ofxConnexion::connexionData;
	
	float zoomVelocity = -data.translation[1] * zoomSpeed;
	curZoom += zoomVelocity;
	curZoom = ofClamp(curZoom, minZoom, maxZoom);
	ofxVec3f& avg = Particle::avg;
	
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
	gluLookAt(0, 0, curZoom,
						avg.x, avg.y, avg.z,
						0, 1, 0);
	
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

float ConnexionCamera::getZoom() {
	return curZoom;
}