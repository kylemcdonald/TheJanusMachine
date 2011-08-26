#include "testApp.h"

void testApp::setup() {
	pointBrightness = .5;
	aberration = 3;
	aperture = .01;
	
	dofShader.load("DOFCloud");

	Particle::setup();

	int n = 50000;
	float radius = 250;
	for(int i = 0; i < n; i++)
		Particle::particles.push_back(Particle(radius));

	isMousePressed = false;
	
	chroma.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
}

void testApp::update() {
	if(!isMousePressed)
		Particle::updateAll(1.4);
}

void testApp::draw() {
	ofBackground(0, 0, 0);
	
	chroma.begin();
	ofClear(0, 0, 0, 1);

	ofPushMatrix();

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	
	// super helpful: http://pmviewer.sourceforge.net/method.php
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);

	ofVec3f& avg = Particle::avg;
	gluLookAt(
		0, 0, 1600,
		avg.x, avg.y, avg.z,
		0, 1, 0);

	glRotatef(ofGetElapsedTimef() * 50, 0, 1, 0);

	float distance = avg.distance(ofPoint(0, 0, 1600));

	glColor4f(1, 1, 1, pointBrightness);

	dofShader.begin();
	dofShader.setUniform1f("focusDistance", distance);
	dofShader.setUniform1f("aperture", aperture);
	
	Particle::drawAll();

	dofShader.end();

	ofPopMatrix();
	
	ofClearAlpha();
	chroma.end();
	
	glColor3f(1, 0, 0);
	chroma.draw(-aberration, 0, chroma.getWidth() + aberration, chroma.getHeight());
	glColor3f(0, 1, 0);
	chroma.draw(0, 0);
	glColor3f(0, 0, 1);
	chroma.draw(0, 0, chroma.getWidth() + aberration, chroma.getHeight());
	
	ofSetColor(255, 255, 255, 255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}

void testApp::mousePressed(int x, int y, int button) {
	isMousePressed = true;
}

void testApp::mouseReleased(int x, int y, int button) {
	isMousePressed = false;
}

void testApp::keyPressed(int key) {
	if(key == 'f')
		ofToggleFullscreen();
}
