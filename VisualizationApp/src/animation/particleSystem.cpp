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
	
	int nVisible = 0;
	
	for(int i = 0; i < particles.size(); i++){
		if (particles[i].bVisible == true){
			
			avgPosition += particles[i].position;
			avgVelocity += particles[i].velocity;
			nVisible++;
		}
	}

	if (nVisible > 0){
		avgPosition /= (float)nVisible;
		avgVelocity /= (float)nVisible;
	}
	
	ofxVec3f diffPosition, diffVelocity;
	for (int i = 0; i < particles.size(); i++){
		if (particles[i].bVisible == true){
			diffPosition = particles[i].position - avgPosition;
			diffVelocity = particles[i].velocity - avgVelocity;
			stdDevPosition += diffPosition * diffPosition;
			stdDevVelocity += diffVelocity * diffVelocity;
		}
	}
	
	if (nVisible > 0){
		stdDevPosition /= (float)nVisible;
		stdDevVelocity /= (float)nVisible;
		
		stdDevPosition.x = sqrt(stdDevPosition.x);
		stdDevPosition.y = sqrt(stdDevPosition.y);
		stdDevPosition.z = sqrt(stdDevPosition.z);
		
		stdDevVelocity.x = sqrt(stdDevVelocity.x);
		stdDevVelocity.y = sqrt(stdDevVelocity.y);
		stdDevVelocity.z = sqrt(stdDevVelocity.z);
	}
				
	
	
}


void particleSystem::drawAll() {
	glBegin(GL_POINTS);
	for(int i = 0; i < particles.size(); i++)
		particles[i].draw();
	glEnd();
}

void particleSystem::updateAll(){
	/*
	Particle::avg.set(0, 0, 0);
	ofxVec3f sum;
	for(int i = 0; i < particles.size(); i++) {
		//particles[i].update();
		sum += particles[i].position;
	}
	Particle::avg = sum / particles.size();
	 */
}

void particleSystem::updateAll(float turbulence) {
	Particle::avg.set(0, 0, 0);
	ofxVec3f sum;
	int nVisible = 0;
	float timeNow = ofGetElapsedTimef();
	for(int i = 0; i < particles.size(); i++) {
		if (particles[i].bVisible) {
			particles[i].update();
			particles[i].updateQueue(timeNow);
			sum += particles[i].position;
			nVisible++;
		}
	}
	Particle::avg = sum / nVisible;
	Particle::globalOffset += turbulence / Particle::neighborhood;
}
