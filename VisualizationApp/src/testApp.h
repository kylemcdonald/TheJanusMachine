#pragma once

#include "ofMain.h"
#include "Particle.h"

#include "ofxShader.h"
#include "ofxFbo.h"
#include "ofxConnexion.h"
#include "ofxControlPanel.h"
#include "ofxDaito.h"
#include "ofxVectorMath.h"

const ofxVec3f xunit3f(1, 0, 0), yunit3f(0, 1, 0), zunit3f(0, 0, 1);

class testApp : public ofBaseApp {
public:
	void setup();
	void exit();
	void update();
	void draw();
	void drawWithAberration();
	
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	bool isMousePressed;

	ofxShader dofShader;
	ofxFbo chroma;
	ofTexture tex;
	
	float pointBrightness, aberration, aperture;
	
	ofxQuaternion lastOrientationVelocity, curOrientation;
};
