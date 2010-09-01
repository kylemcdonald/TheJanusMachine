#pragma once

#include "ofMain.h"
#include "Particle.h"

#include "ofxShader.h"
#include "ofxFbo.h"
#include "ofxControlPanel.h"
#include "ofxDaito.h"
#include "scanPlayer.h"
#include "ofxVectorMath.h"
#include "ConnexionCamera.h"
#include "particleSystem.h"

class testApp : public ofBaseApp {
public:

	void setupControlPanel();

	void setup();
	void exit();
	void update();
	void draw();
	
	void drawWithoutAberration();
	void drawWithAberration();
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseDragged(int x, int y, int button);

	bool isMousePressed;
	
	float appFps;

	ofxShader dofShader;
	ofxFbo chroma;
	ofTexture tex;
	
	ofxControlPanel panel;
	
	particleSystem PS;
	
	float pointBrightness, aberration, aperture;
	scanPlayer SP;
	ConnexionCamera connexionCamera;
	
	bool bTogglePlayer;
	

};
