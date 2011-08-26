#include "Particle.h"

vector<Particle>
	Particle::particles;

ofVec3f
	Particle::centeringForce,
	Particle::globalOffset,
	Particle::avg;

ofxMSAPerlin
	Particle::perlin;

float
	Particle::speed,
	Particle::spread,
	Particle::viscosity,
	Particle::independence,
	Particle::neighborhood;

void Particle::setup() {
	perlin.setup(4, 1, .5, (int) ofRandom(0, 1000));
	globalOffset.set(0, 1. / 3, 2. / 3);
	speed = 24;
  spread = 100;
  viscosity = .1;
  independence = .15;
  neighborhood = 700;
}

void Particle::drawAll() {
	glBegin(GL_POINTS);
	for(int i = 0; i < particles.size(); i++)
		particles[i].draw();
	glEnd();
}

void Particle::updateAll(float turbulence) {
	avg.set(0, 0, 0);
	ofVec3f sum;
	for(int i = 0; i < particles.size(); i++) {
		particles[i].update();
		sum += particles[i].position;
	}
	avg = sum / particles.size();
	globalOffset += turbulence / neighborhood;
}
