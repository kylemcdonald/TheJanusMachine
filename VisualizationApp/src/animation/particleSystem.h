#pragma once

#include "ofMain.h"
#include "ofxVec3f.h"
#include "Particle.h"


class particleSystem {
	public:

	
	vector<Particle> particles;
	
	void setup();
	void drawAll();
	void updateAll(float turbulence);
	void updateAll();

};
