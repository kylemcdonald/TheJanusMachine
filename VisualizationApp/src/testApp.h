#pragma once

#include "ofMain.h"
#include "Particle.h"

#include "ofShader.h"
#include "ofFbo.h"
#include "ofxControlPanel.h"
#include "ofxDaito.h"
#include "scanPlayer.h"
#include "ofVectorMath.h"
#include "ConnexionCamera.h"
#include "particleSystem.h"

#include "scanNotifier.h"

typedef enum{
	VIZAPP_PARTICLES_FREE,
	VIZAPP_NEWFACE,
	VIZAPP_PARTICLES_BREAK_APART,
	VIZAPP_PARTICLES_FACE,
	VIZAPP_UNLOADFACE
}vizState;

class testApp : public ofBaseApp {
public:
	static const int targetWidth = 1280;
	static const int targetHeight = 720;

	void setupControlPanel();

	void setup();
	void exit();
	void update();
	void draw();
	void standardDraw();
	
	void eventsIn(eventStruct &dataIn);
	
	bool beginParticleMoveToTarget(string mode);
	void beginParticleBreakApart(string mode);
	void setParticlesFromFace();
	void updateFreeParticles();
	
	void drawWithoutAberration();
	void drawWithAberration();
	void keyPressed(int key);
	void keyReleased(int key);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	
	void daitoPrintout();
	
	void connexionEvent(ConnexionData& data);
	float timeLastLoaded;

	bool isMousePressed;
	bool bDoUnload;
	
	bool bJustLoadedUser;
	
	float appFps;

	ofShader dofShader, sphereShader;
	ofFbo chroma;
	
	string lastFolder;
	
	string currentMsg;
	
	vizState state;
	
	scanNotifier notifier;
	
	
	ofxControlPanel panel;
	
	int frameW, frameH;
	float frameScaleFactor;
	
	
	particleSystem PS;
	
	float pointBrightness, aberration, aperture;
	scanPlayer SP;
	ConnexionCamera connexionCamera;
	
	bool bTogglePlayer;
	
	bool isSlow;
	float slowState;
	
	bool doScreenshots;
	string screenshotFolder;
	int screenshotCount;
	float lastFrame;
};
