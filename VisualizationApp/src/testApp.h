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

#include "scanNotifier.h"

typedef enum{
	VIZAPP_PARTICLES_FREE,
	VIZAPP_NEWFACE,
	VIZAPP_PARTICLES_FACE,
	VIZAPP_UNLOADFACE
}vizState;

class testApp : public ofBaseApp {
public:

	void setupControlPanel();

	void setup();
	void exit();
	void update();
	void draw();
	
	void eventsIn(eventStruct &dataIn);
	
	bool beginParticleMoveToTarget(string mode);
	void beginParticleBreakApart(string mode);
	void setParticlesFromFace();
	void updateFreeParticles();
	
	void drawWithoutAberration();
	void drawWithAberration();
	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseDragged(int x, int y, int button);

	bool isMousePressed;
	bool bDoUnload;
	
	float appFps;

	ofxShader dofShader, sphereShader;
	ofxFbo chroma;
	ofTexture tex;
	
	vizState state;
	
	scanNotifier notifier;
	
	
	ofxControlPanel panel;
	
	int frameW, frameH;
	
	particleSystem PS;
	
	float pointBrightness, aberration, aperture;
	scanPlayer SP;
	ConnexionCamera connexionCamera;
	
	bool bTogglePlayer;
	

};
