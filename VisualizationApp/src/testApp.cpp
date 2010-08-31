#include "testApp.h"

void testApp::setup() {
	ofxDaito::setup("oscSettings.xml");
	ofxConnexion::start("VisualizationApp");
	ofxConnexion::setLed(false);
	
	pointBrightness = .5;
	aberration = .02;
	aperture = .01;
	
	dofShader.setup("shaders/DOFCloud");

	Particle::setup();

	int n = 50000;
	float radius = 250;
	for(int i = 0; i < n; i++)
		Particle::particles.push_back(Particle(radius));

	isMousePressed = false;
	
	chroma.setup(ofGetWidth(), ofGetHeight(), false);
	tex.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
	chroma.attach(tex);
	
	curZoom = 1600;
	minZoom = 400;
	maxZoom = 3200;
}

void testApp::exit() {
	ofxConnexion::stop();
}

void testApp::update() {
	if(!isMousePressed)
		Particle::updateAll(1.4);
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
	
	ConnexionData& data = ofxConnexion::connexionData;
	
	float zoomSpeed = 1;
	float zoomVelocity = -data.translation[2] * zoomSpeed;
	curZoom += zoomVelocity;
	curZoom = ofClamp(curZoom, minZoom, maxZoom);
	ofxVec3f& avg = Particle::avg;
	gluLookAt(
		0, 0, curZoom,// 1600,
		avg.x, avg.y, avg.z,
		0, 1, 0);
	
	/*
	glTranslatef(
							10 * data.translation[0],
							10 * data.translation[1],
							10 * -data.translation[2]);
	*/
	
	// rotation speed should technically be tied to the fps
	float rotationSpeed = .001;
	
	// some of these things are negative, but might be different
	// depending on how you've configured your space navigator
	ofxQuaternion curOrientationVelocity;
	curOrientationVelocity.makeRotate(-data.rotation[0] * rotationSpeed, xunit3f,
																		-data.rotation[1] * rotationSpeed, yunit3f,
																		+data.rotation[2] * rotationSpeed, zunit3f);
	
	float momentum = .9;
	lastOrientationVelocity.slerp(momentum, curOrientationVelocity, lastOrientationVelocity);
	
	curOrientation *= lastOrientationVelocity;
	float amount;
	ofxVec3f angle;
	curOrientation.getRotate(amount, angle);
	glRotatef(ofRadToDeg(amount), angle.x, angle.y, angle.z);
	
	//glRotatef(ofGetElapsedTimef() * 50, 0, 1, 0);

	float distance = avg.distance(ofPoint(0, 0, 1600));

	glColor4f(1, 1, 1, pointBrightness);

	dofShader.begin();
	dofShader.setUniform("focusDistance", distance);
	dofShader.setUniform("aperture", aperture);
	
	Particle::drawAll();

	dofShader.end();

	ofPopMatrix();
	
	chroma.end();
	chroma.clearAlpha();
	
	drawWithAberration();
	
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
