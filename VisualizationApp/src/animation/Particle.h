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
	
	static ofxVec3f centeringForce, globalOffset, avg;
	static float speed, spread, viscosity, independence, rebirthRadius, neighborhood;
	
    ofxVec3f position, velocity, force, localOffset;
	bool bVisisble;
	
  Particle(float radius) {
    randomize(localOffset);
  	randomize(position);
  	position *= radius;
	  bVisisble = true;
  }
  inline void draw() {
	  if (bVisisble){
  	glVertex3fv(position.v);
	  }
  }
  inline void applyFlockingForce()   {
	  
	  
	  float scale = 0.1f;
		float basex = (position.x / neighborhood)*scale;
		float basey = (position.y / neighborhood)*scale;
		float basez = (position.z / neighborhood)*scale;
    force.x +=
     ofSignedNoise(
        basex + (globalOffset.x + localOffset.x * independence),
        basey,
        basez)*0.5;
		force.y +=
     ofSignedNoise(
        basex,
        basey + (globalOffset.y  + localOffset.y * independence),
        basez)*0.5;
		force.z +=
     ofSignedNoise(
        basex,
        basey,
        basez + (globalOffset.z + localOffset.z * independence))*0.5;
  }
  inline void applyViscosityForce() {
    force += velocity * -viscosity;
  }
  inline void applyCenteringForce() {
    centeringForce.set(position);
    centeringForce -= avg;
    float distanceToCenter = centeringForce.length();
    centeringForce.normalize();
    centeringForce *= -distanceToCenter / (spread * spread);
    force += centeringForce;
  }
  inline void update() {
    force.set(0, 0, 0);
    applyFlockingForce();
    applyViscosityForce();
		applyCenteringForce();
    velocity += force; // mass = 1
    position += velocity * speed;
  }
};
