#pragma once

#include "ofMain.h"
#include "ofxVec3f.h"
#include "ofxVec4f.h"
#include "ofxMSAPerlin.h"

inline void randomize(ofxVec3f& v) {
	v.x = ofRandomf();
	v.y = ofRandomf();
	v.z = ofRandomf();
	v.normalize();
}

typedef enum{
	PARTICLE_FREE,
	PARTICLE_FLOCKING,
	PARTICLE_EXPLODE,
	PARTICLE_TARGET,
	PARTICLE_REST
}particleState;

struct timedState{
	
	timedState(particleState _state, float time){
		state			= _state;
		timeTill		= time;
	}
	
	particleState state;
	float timeTill;
};

class Particle {
	public:
		
		//------------------------------------------------
		static ofxVec3f centeringForce, globalOffset, avg;
		static float speed, spread, viscosity, independence, rebirthRadius, neighborhood, targetForce;
		static float noiseScaleInput, noiseScaleOutput;
		
		//------------------------------------------------
		ofxVec4f color;
		ofxVec3f position, velocity, explodeForce, force, localOffset;
		bool bVisible;
		ofxVec3f targetPosition;
		
		particleState state;
		
		Particle(float radius);
		void draw();
		void applyFlockingForce(bool bAccountForTargetForce = true);
		void applyViscosityForce();
		void applyCenteringForce();
		void applyTargetForce();

		void startState(particleState newState);

		void queueState(particleState stateIn, float timeToStartState);
		void updateQueue(float timeInF);
		void clearQueueState();
		
		vector <timedState> stateQueue;
		
		void update();
};
