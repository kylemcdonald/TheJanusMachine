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

void particleSystem::calculate(){
	
	avgPosition.set(0,0,0);
	avgVelocity.set(0,0,0);
	stdDevPosition.set(0,0,0);
	stdDevVelocity.set(0,0,0);
	
	for(int i = 0; i < particles.size(); i++){
		avgPosition += particles[i].position;
		avgVelocity += particles[i].velocity;
	}

	avgPosition /= (float)particles.size();
	avgVelocity /= (float)particles.size();
	
	
	for (int i = 0; i < particles.size(); i++){
		stdDevPosition += ( particles[i].position - avgPosition) * ( particles[i].position - avgPosition);
		stdDevVelocity += ( particles[i].velocity - avgVelocity) * ( particles[i].velocity - avgVelocity);
	}
	
	stdDevPosition /= (float)particles.size();
	stdDevVelocity /= (float)particles.size();
	
	stdDevPosition.x = sqrt(stdDevPosition.x);
	stdDevPosition.y = sqrt(stdDevPosition.y);
	stdDevPosition.z = sqrt(stdDevPosition.z);
	
	stdDevVelocity.x = sqrt(stdDevVelocity.x);
	stdDevVelocity.y = sqrt(stdDevVelocity.y);
	stdDevVelocity.z = sqrt(stdDevVelocity.z);
	
				
	
	
}

float avgPosition;
float avgVelocity;
float stdDeviationPosition;
float stdDeviationVelocity;



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
