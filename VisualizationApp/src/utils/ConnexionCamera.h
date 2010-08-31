#pragma once

#include "ofxConnexion.h"
#include "ofxVectorMath.h"
#include "Particle.h"

const ofxVec3f xunit3f(1, 0, 0), yunit3f(0, 1, 0), zunit3f(0, 0, 1);

class ConnexionCamera {
public:
	ConnexionCamera();
	void draw();
protected:
	float curZoom, minZoom, maxZoom;
	float zoomSpeed;
	float rotationMomentum, rotationSpeed;
	ofxQuaternion lastOrientationVelocity, curOrientation;
};