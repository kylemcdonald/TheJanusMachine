#pragma once

#include "ofMain.h"
#include "Particle.h"

#include "ofxShader.h"
#include "ofxFbo.h"
#include "ofxConnexion.h"
#include "ofxControlPanel.h"
#include "ofxDaito.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void exit();
	void update();
	void draw();

	void ConnexionEvent(ConnexionData& data);
	
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	bool isMousePressed;

	ofxShader dofShader;
	ofxFbo chroma;
	ofTexture tex;
	
	float pointBrightness, aberration, aperture;
	
	ConnexionData data;
};
