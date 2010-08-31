#pragma once

#include "ofMain.h"
#include "ofxVec3f.h"
#include "ofxMSAPerlin.h"

inline void randomize(ofxVec3f& v) {
	v.x = ofRandomf();
	v.y = ofRandomf();
	v.z = ofRandomf();
	v.normalize();
}

class Particle {
	public:
		
		//------------------------------------------------
		static ofxVec3f centeringForce, globalOffset, avg;
		static float speed, spread, viscosity, independence, rebirthRadius, neighborhood, targetForce;
		
		//------------------------------------------------
		ofxVec3f color;
		ofxVec3f position, velocity, force, localOffset;
		bool bVisisble;
		ofxVec3f targetPosition;
		
		Particle(float radius);
		void draw();
		void applyFlockingForce();
		void applyViscosityForce();
		void applyCenteringForce();
		void applyTargetForce();
		void update();
};
