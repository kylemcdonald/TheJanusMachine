#pragma once

#include "ofMain.h"
#include "ofxMSAPerlin.h"

inline void randomize(ofVec3f& v) {
	v.x = ofRandomf();
	v.y = ofRandomf();
	v.z = ofRandomf();
	v.normalize();
}

class Particle {
public:
	static ofVec3f centeringForce, globalOffset, avg;
	static ofxMSAPerlin perlin;
	static float speed, spread, viscosity, independence, rebirthRadius, neighborhood;
	static vector<Particle> particles;
	static void setup();
  static void drawAll();
  static void updateAll(float turbulence);

  ofVec3f position, velocity, force, localOffset;
  Particle(float radius) {
    randomize(localOffset);
  	randomize(position);
  	position *= radius;
  }
  inline void draw() {
  	glVertex3f(position.x, position.y, position.z);
  }
  inline void applyFlockingForce() {
		float basex = position.x / neighborhood;
		float basey = position.y / neighborhood;
		float basez = position.z / neighborhood;
    force.x +=
      perlin.get(
				basex + globalOffset.x + localOffset.x * independence,
				basey,
				basez);
		force.y +=
      perlin.get(
				basex,
				basey + globalOffset.y  + localOffset.y * independence,
				basez);
		force.z +=
      perlin.get(
				basex,
				basey,
				basez + globalOffset.z + localOffset.z * independence);
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
