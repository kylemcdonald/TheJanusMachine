#pragma once

#include "ofxConnexion.h"
#include "ofxVectorMath.h"
#include "Particle.h"
#include "ofMain.h"
#include "particleSystem.h"

enum {START_RESET, FREE_MOVE};

const ofxVec3f xunit3f(1, 0, 0), yunit3f(0, 1, 0), zunit3f(0, 0, 1);

class ConnexionCamera {
public:
	ConnexionCamera();
	void setup(particleSystem& PS);
	void draw(float mouseX, float mouseY);
	float getZoom();
	void moveZoom(float change);
	void addRotation(ofxQuaternion rotation);
	
	void update();
	float lastFrameZoom;
	ofxQuaternion lastOrientation;
	ofxQuaternion rotateForOSC;
	float amount;
	ofxVec3f angle;
	
	
	float zoomSpeed;
	float rotationSpeed;
	float zoomChangeAmount;
	float quaternionChangeAmount;
	
	float positionMomentum, zoomMomentum, rotationMomentum;

	float minZoom, maxZoom;
	
	void startReset();
	float resetLength, resetDelay;
	
	static float baseZoom;
	
	float zoomScaleFactor;
	
protected:
	int mode;
	float resetStart;
	ofxQuaternion startOrientation;
	float startZoom;
	
	float curZoom;
	ofxQuaternion lastOrientationVelocity, curOrientation;	
	
	ofxVec3f lastAvg, lastDev;
	particleSystem* PS;
	
	float lastMovement;
};