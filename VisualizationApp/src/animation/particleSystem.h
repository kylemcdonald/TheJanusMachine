#pragma once

#include "ofMain.h"
#include "ofVec3f.h"
#include "Particle.h"


class particleSystem {
	public:

	
	vector<Particle> particles;
	
	void setup(int numParticles);
	void drawAll();
	void updateAll(float turbulence);
	void updateAll();
	
	void calculate();
	
	ofVec3f avgPosition;
	ofVec3f avgVelocity;
	ofVec3f stdDevPosition;
	ofVec3f stdDevVelocity;
	
	float pctLocked;
	

};
