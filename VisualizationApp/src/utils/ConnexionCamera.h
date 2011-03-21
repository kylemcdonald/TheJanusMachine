#pragma once

#include "ofxConnexion.h"
#include "ofVectorMath.h"
#include "Particle.h"
#include "ofMain.h"
#include "particleSystem.h"

enum {START_RESET, FREE_MOVE};

const ofVec3f xunit3f(1, 0, 0), yunit3f(0, 1, 0), zunit3f(0, 0, 1);

class ConnexionCamera {
public:
	ConnexionCamera();
	void setup(particleSystem& PS);
	void draw(float mouseX, float mouseY, bool canReset);
	float getZoom();
	void moveZoom(float change);
	void addRotation(ofQuaternion rotation);
	
	void update();
	float lastFrameZoom;
	ofQuaternion lastOrientation;
	ofQuaternion rotateForOSC;
	float amount;
	ofVec3f angle;
	
	bool persistentMode;
	
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
	ofQuaternion startOrientation;
	ofQuaternion baseOrientation;
	float startZoom;
	
	float curZoom;
	ofQuaternion lastOrientationVelocity, curOrientation;	
	
	ofVec3f lastAvg, lastDev;
	particleSystem* PS;
	
	float lastMovement;
};