#pragma once

#include "ofMain.h"
#include "ofxShader.h"
#include "Particle.h"
#include "ofxFbo.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	bool isMousePressed;

	ofxShader dofShader;
	ofxFbo chroma;
	ofTexture tex;
	
	float pointBrightness, aberration, aperture;
};
