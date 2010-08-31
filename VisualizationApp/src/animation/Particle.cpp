#include "Particle.h"


ofxVec3f
	Particle::centeringForce,
	Particle::globalOffset,
	Particle::avg;


float
	Particle::speed,
	Particle::spread,
	Particle::viscosity,
	Particle::independence,
	Particle::neighborhood,
	Particle::targetForce;




Particle::Particle(float radius) {
    randomize(localOffset);
  	randomize(position);
  	position *= radius;
	bVisisble = true;
	color.set(1,1,1);
}


void Particle::draw() {
	if (bVisisble){
		glColor3fv(color.v);
		glVertex3fv(position.v);
	}
}
void Particle::applyFlockingForce()   {
	
	
	float scale = 0.1f;
	float basex = (position.x / neighborhood)*scale;
	float basey = (position.y / neighborhood)*scale;
	float basez = (position.z / neighborhood)*scale;
	
	ofxVec3f addToForce;
	
    addToForce.x +=
	ofSignedNoise(
				  basex + (globalOffset.x + localOffset.x * independence),
				  basey,
				  basez)*0.5;
	addToForce.y +=
	ofSignedNoise(
				  basex,
				  basey + (globalOffset.y  + localOffset.y * independence),
				  basez)*0.5;
	addToForce.z +=
	ofSignedNoise(
				  basex,
				  basey,
				  basez + (globalOffset.z + localOffset.z * independence))*0.5;


	addToForce *= (1-targetForce);
	
	force += addToForce;
	
}


void Particle::applyViscosityForce() {
    force += velocity * -viscosity;
}

void Particle::applyTargetForce() {
	ofxVec3f diff = targetPosition - position;
	//diff.normalize();
	diff /= 100.0;
	
	force += (diff * targetForce);
}


void Particle::applyCenteringForce() {
    centeringForce.set(position);
    centeringForce -= avg;
    float distanceToCenter = centeringForce.length();
    centeringForce.normalize();
    centeringForce *= -distanceToCenter / (spread * spread);
    force += centeringForce;
}

void Particle::update() {
    force.set(0, 0, 0);
    applyFlockingForce();
    applyViscosityForce();
	applyCenteringForce();
	applyTargetForce();
	
    velocity += force; // mass = 1
    position += velocity * speed;
}