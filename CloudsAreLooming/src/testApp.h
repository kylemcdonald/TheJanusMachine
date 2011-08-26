#pragma once

#include "ofMain.h"
#include "Particle.h"

class testApp : public ofBaseApp {
public:
	void setup();	
	void update();
	void draw();
	void keyPressed(int key);

	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	bool isMousePressed;

	ofShader dofShader;
	ofFbo chroma;
	
	float pointBrightness, aberration, aperture;
};
