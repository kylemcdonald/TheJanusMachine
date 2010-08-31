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
	Particle::neighborhood;
