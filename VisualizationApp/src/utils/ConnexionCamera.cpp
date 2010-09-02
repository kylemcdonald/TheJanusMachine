#include "ConnexionCamera.h"


// this is for update;
float rotationDistance(ofxQuaternion& from, ofxQuaternion& to) {
	
	float dist = from._v[0]*to._v[0] + from._v[1]*to._v[1] +  from._v[2]*to._v[2] +  from._v[3]*to._v[3];
	
	if (dist > 0.0001){
		dist = 1/dist;	
	} else {
		dist = 1/0.0001;
	}
	return (dist-1)*10.0;
}


ConnexionCamera::ConnexionCamera() :
	curZoom(1600),
	minZoom(400),
	maxZoom(3200),
	zoomSpeed(1),
	rotationSpeed(.001),
	rotationMomentum(.9),
	zoomMomentum(.99),
	positionMomentum(.99) {
	
	devScale = .001;
	lastFrameZoom = 0;
}

void ConnexionCamera::setup(particleSystem& PS) {
	this->PS = &PS;
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
	
	float zoomVelocity = -data.translation[2] * zoomSpeed;
	moveZoom(zoomVelocity);
	
	ofxVec3f& curDev = PS->stdDevPosition;
	lastDev = curDev.interpolate(lastDev, zoomMomentum);
	float avgdev = (lastDev.x + lastDev.y + lastDev.z) / 3.;
	avgdev *= devScale;
	glTranslatef(ofGetWidth() / 2, ofGetHeight() / 2, -curZoom * avgdev);
	
	// (rotation speed should technically be affected by the fps)
	ofxQuaternion curOrientationVelocity;
	curOrientationVelocity.makeRotate(-data.rotation[0] * rotationSpeed, xunit3f,
																			+data.rotation[2] * rotationSpeed, yunit3f,
																			+data.rotation[1] * rotationSpeed, zunit3f);
		
	lastOrientationVelocity.slerp(rotationMomentum, curOrientationVelocity, lastOrientationVelocity);
	
	curOrientation *= lastOrientationVelocity;
	curOrientation.getRotate(amount, angle);
	glRotatef(ofRadToDeg(amount), angle.x, angle.y, angle.z);
	
	ofxVec3f& curAvg = Particle::avg;
	lastAvg = curAvg.interpolate(lastAvg, positionMomentum);
	glTranslatef(-lastAvg.x, -lastAvg.y, -lastAvg.z);
}

float ConnexionCamera::getZoom() {
	return curZoom;
}

void ConnexionCamera::moveZoom(float change) {
	if(change != 0)
		lastMovement = ofGetElapsedTimef();
	curZoom += change;
	curZoom = ofClamp(curZoom, minZoom, maxZoom);
}