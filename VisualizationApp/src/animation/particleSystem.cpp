#include "particleSystem.h"



void particleSystem::setup(int numParticles) {
	
	Particle::globalOffset.set(0, 1. / 3, 2. / 3);
	Particle::speed = 24;
	Particle::spread = 100;
	Particle::viscosity = .1;
	Particle::independence = .15;
	Particle::neighborhood = 700;
	Particle::targetForce = 0;
	
	float radius = 250;
	for(int i = 0; i < numParticles; i++){
		particles.push_back(Particle(radius));
	}
}

void particleSystem::drawAll() {
	glBegin(GL_POINTS);
	for(int i = 0; i < particles.size(); i++)
		particles[i].draw();
	glEnd();
}

void particleSystem::updateAll(){
	Particle::avg.set(0, 0, 0);
	ofxVec3f sum;
	for(int i = 0; i < particles.size(); i++) {
		//particles[i].update();
		sum += particles[i].position;
	}
	Particle::avg = sum / particles.size();
}

void particleSystem::updateAll(float turbulence) {
	Particle::avg.set(0, 0, 0);
	ofxVec3f sum;
	float timeNow = ofGetElapsedTimef();
	for(int i = 0; i < particles.size(); i++) {
		particles[i].update();
		particles[i].updateQueue(timeNow);
		sum += particles[i].position;
	}
	Particle::avg = sum / particles.size();
	Particle::globalOffset += turbulence / Particle::neighborhood;
}
