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
	
	cout << "setup() is in thread " << pthread_self() << endl;
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

	ofxVec3f& avg = Particle::avg;
	gluLookAt(
		0, 0, 1600,
		avg.x, avg.y, avg.z,
		0, 1, 0);
	
	// some of these things are negative, but might be different
	// depending on how you've configured your space navigator
	
	ConnexionData& data = ofxConnexion::connexionData;
	
	glTranslatef(
							10 * data.translation[0],
							10 * data.translation[1],
							10 * -data.translation[2]);
	ofRotateX(-data.rotation[0]);
	ofRotateY(-data.rotation[1]);
	ofRotateZ(data.rotation[2]);
	
	glRotatef(ofGetElapsedTimef() * 50, 0, 1, 0);

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
}

void testApp::drawWithAberration() {
	float scaleFactor;
	
	glColor3f(1, 0, 0);
	glPushMatrix();
	scaleFactor = 1 - aberration;
	glTranslatef(chroma.getWidth() / 2, chroma.getHeight() / 2, 0);
	glScalef(scaleFactor, scaleFactor, 1);
	chroma.draw(-chroma.getWidth() / 2, -chroma.getHeight() / 2);
	glPopMatrix();
	
	glColor3f(0, 1, 0);
	chroma.draw(0, 0);
	
	glColor3f(0, 0, 1);
	glPushMatrix();
	scaleFactor = 1 + aberration;
	glTranslatef(chroma.getWidth() / 2, chroma.getHeight() / 2, 0);
	glScalef(scaleFactor, scaleFactor, 1);
	chroma.draw(-chroma.getWidth() / 2, -chroma.getHeight() / 2);
	glPopMatrix();
	
	ofSetColor(255, 255, 255, 255);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
}

void testApp::mousePressed(int x, int y, int button) {
	isMousePressed = true;
}

void testApp::mouseReleased(int x, int y, int button) {
	isMousePressed = false;
}
