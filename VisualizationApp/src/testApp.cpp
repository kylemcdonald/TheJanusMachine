#include "testApp.h"

void testApp::setup() {
	
	
	ofxDaito::setup("oscSettings.xml");
	ofxConnexion::start("VisualizationApp");
	ofxConnexion::setLed(false);
	
	//connexionCamera.
	
	SP.setup();
	SP.loadDirectory("input/otherTest");
	
	
	pointBrightness = .5;
	aberration = .02;
	aperture = .01;
	
	dofShader.setup("shaders/DOFCloud");

	PS.setup();

	
	isMousePressed = false;
	
	chroma.setup(ofGetWidth(), ofGetHeight(), false);
	tex.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
	chroma.attach(tex);
}

void testApp::keyPressed(int key){
	
	if (key == ' '){
		
	}
}

void testApp::exit() {
	ofxConnexion::stop();
}

void testApp::update() {
	
	SP.update();
	
	if(isMousePressed) {
		PS.updateAll(1.4);
	
	} else {
	
		if (SP.TSL.state == TH_STATE_LOADED){
			if (SP.totalNumFrames > 0){
				
				unsigned char * pixels = SP.TSL.depthImageFrames[SP.currentFrame].getPixels();
				for (int i = 0; i < 320; i++){
					for (int j = 0; j < 240; j++){
						float zposition = pixels[(j*320+i)*4 + 3];
						PS.particles[j*320+i].position.set(i*3 - 320*3/2, j*3-240*3/2, zposition*3);
						if (zposition == 0){
							PS.particles[j*320+i].bVisisble = false;
						} else {
							PS.particles[j*320+i].bVisisble = true;
						}
					}
				}
				
			}
		}
		
		PS.updateAll();
	}
}

void testApp::draw() {
	//aberration = ofMap(mouseX, 0, ofGetWidth(), 0, 1);
	//pointBrightness = ofMap(mouseY, 0, ofGetHeight(), 0, 1);
	
	ofBackground(0, 0, 0);
	
	chroma.begin();
	chroma.setBackground(0, 0, 0, 1);

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
	
	connexionCamera.draw();
	
	//glRotatef(ofGetElapsedTimef() * 50, 0, 1, 0);
	
	ofxVec3f& avg = Particle::avg;
	float distance = avg.distance(ofPoint(0, 0, 1600));

	glColor4f(1, 1, 1, pointBrightness);

	dofShader.begin();
	dofShader.setUniform("focusDistance", distance);
	dofShader.setUniform("aperture", aperture);
	
	PS.drawAll();

	dofShader.end();

	ofPopMatrix();
	
	chroma.end();
	chroma.clearAlpha();
	
	drawWithAberration();
	
	SP.draw();
	
	ofSetColor(255, 255, 255, 255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}

void testApp::drawWithoutAberration() {
	chroma.draw(0, 0);
}

void testApp::drawWithAberration() {
	float scaleFactor;
	
	// red
	glColor3f(1. / 2, 0, 0);
	glPushMatrix();
	scaleFactor = 1 - aberration;
	glTranslatef(chroma.getWidth() / 2, chroma.getHeight() / 2, 0);
	glScalef(scaleFactor, scaleFactor, 1);
	chroma.draw(-chroma.getWidth() / 2, -chroma.getHeight() / 2);
	glPopMatrix();
	
	// yellow
	glColor3f(1. / 2, 1. / 3, 0);
	glPushMatrix();
	scaleFactor = 1 - aberration / 2;
	glTranslatef(chroma.getWidth() / 2, chroma.getHeight() / 2, 0);
	glScalef(scaleFactor, scaleFactor, 1);
	chroma.draw(-chroma.getWidth() / 2, -chroma.getHeight() / 2);
	glPopMatrix();
	
	// green
	glColor3f(0, 1. / 3, 0);
	chroma.draw(0, 0);
	
	// cyan
	glColor3f(0, 1. / 3, 1. / 2);
	glPushMatrix();
	scaleFactor = 1 + aberration / 2;
	glTranslatef(chroma.getWidth() / 2, chroma.getHeight() / 2, 0);
	glScalef(scaleFactor, scaleFactor, 1);
	chroma.draw(-chroma.getWidth() / 2, -chroma.getHeight() / 2);
	glPopMatrix();
	
	// blue
	glColor3f(0, 0, 1. / 2);
	glPushMatrix();
	scaleFactor = 1 + aberration;
	glTranslatef(chroma.getWidth() / 2, chroma.getHeight() / 2, 0);
	glScalef(scaleFactor, scaleFactor, 1);
	chroma.draw(-chroma.getWidth() / 2, -chroma.getHeight() / 2);
	glPopMatrix();
}

void testApp::mousePressed(int x, int y, int button) {
	isMousePressed = true;
}

void testApp::mouseReleased(int x, int y, int button) {
	isMousePressed = false;
}
