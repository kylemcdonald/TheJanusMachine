#include "testApp.h"

bool bDebugMode = true;

char * path = getenv("HOME");
static string userFolder = string(path) + string("/Desktop/");

//--------------------------------------------------------------------------
void testApp::setup() {
	ofxDaito::setup("oscSettings.xml");
	ofxConnexion::start("VisualizationApp");
	ofxConnexion::setLed(false);
	
	SP.setup();
	SP.loadDirectory("input/otherTest");
	
	notifier.setup("network.xml");
	notifier.enable();
	ofAddListener(notifier.theEvent,this,&testApp::eventsIn);		
	
	state = VIZAPP_PARTICLES_FREE;
	
	setupControlPanel();
	if( bDebugMode == false ){
		ofSetFullscreen(true);
		panel.hide();
	}
		
	pointBrightness = .5;
	aberration		= .02;
	aperture		= .01;
	
	dofShader.setup("shaders/DOFCloud");
	sphereShader.setup("shaders/SphereShader");

	frameW = 320;
	frameH = 240;
	int numParticles = frameW * frameH;

	bDoUnload = false;

	PS.setup(numParticles);
	
	isMousePressed = false;
	
	chroma.setup(ofGetWidth(), ofGetHeight(), false);
	chroma.setBackground(0, 0, 0, 255);
	tex.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
	chroma.attach(tex);
	
	bTogglePlayer = panel.getValueB("toggle_mode");
	
	for(int k = 0; k < PS.particles.size(); k++){	
		PS.particles[k].queueState(PARTICLE_FLOCKING,  0.0);
	}
	
	connexionCamera.setup(PS);
	currentMsg = "app started";
	
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------------------
void testApp::setupControlPanel(){

	ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(80, 240, 240, 255));
		
	panel.setup("controls", 0, 0, 300, 750);
	panel.addPanel("general controls", 4, false);
	panel.addPanel("animation controls", 4, false);
	panel.addPanel("render controls", 4, false);
	panel.addPanel("camera params", 4, false);
	
	panel.addPanel("debug params", 4, false);
			
	//--------- general params
	panel.setWhichPanel("general controls");
	panel.setWhichColumn(0);
	
	panel.addToggle("convert to png after load", "bConvertToPng", true);	
	
	panel.addChartPlotter("fps", guiStatVarPointer("app fps", &appFps, GUI_VAR_FLOAT, true, 2), 200, 80, 200, 8, 100);
	
	panel.addToggle("player mode", "toggle_mode", true);

	//--------- animation params
	panel.setWhichPanel("animation controls");
	panel.setWhichColumn(0);	

	panel.addSlider("particle targetForce", "particle_targetForce", 0.0, 0.0, 1.0, false);
	panel.addSlider("noise scale in", "noise_scale_input", 0.1, 0.0, 1.0, false);
	panel.addSlider("noise scale out", "noise_scale_output", 0.5, 0.0, 1.0, false);
	
	panel.addChartPlotter("fps", guiStatVarPointer("app fps", &appFps, GUI_VAR_FLOAT, true, 2), 200, 80, 200, 8, 100);
	
	panel.addSlider("particle speed", "particle_speed", 24, 2, 50, false);
	panel.addSlider("particle spread", "particle_spread", 100, 2, 500, false);
	panel.addSlider("particle viscosity", "particle_viscosity", 0.1, 0.0, 0.5, false);
	panel.addSlider("particle independence", "particle_independence", 0.15, 0.0, 0.8, false);
	panel.addSlider("particle neighborhood", "particle_neighborhood", 700, 100, 2000, false);


	//--------- render params
	panel.setWhichPanel("render controls");
	panel.setWhichColumn(0);
	
	panel.addToggle("do ghetto fbo trails", "do_trails", false);	
	
	panel.addChartPlotter("fps", guiStatVarPointer("app fps", &appFps, GUI_VAR_FLOAT, true, 2), 200, 80, 200, 8, 100);
	
	panel.addSlider("dof focus offset", "focus_offset", 0.0, -1000, 1000, false);
	panel.addSlider("point brightness", "point_brightness", 0.5, 0.1, 30.0, false);
	panel.addSlider("aberration", "aberration", 0.02, 0.005, 0.2, false);
	panel.addSlider("aperture", "aperture", 0.01, 0.001, 0.2, false);
	panel.addSlider("sphere alpha", "sphere_alpha", 0.1, 0.0, 1.0, false);
	
	// - - -- - --- - camera param eters
	panel.setWhichPanel("camera params");
	panel.setWhichColumn(0);
	
	panel.addSlider("rotation momentum", "rotationMomentum", .9, .5, 1, false);
	panel.addSlider("position momentum", "positionMomentum", .99, .5, 1, false);
	panel.addSlider("zoom momentum", "zoomMomentum", .99, .5, 1, false);
	panel.addSlider("fov", "fov", 60, 1, 180, false);
	
	//--------- general params
	panel.setWhichPanel("debug params");
	panel.setWhichColumn(0);
	
	
	panel.addToggle("draw particles", "bDrawParticles", true);	
	
	panel.selectedPanel = 1;
	
	
	
	
	
	panel.loadSettings("appSettings.xml");

	//Particle::globalOffset.set(0, 1. / 3, 2. / 3);

}

void testApp::keyPressed(int key){
	
	if( key == 'f' ){
		ofToggleFullscreen();
	}
	
	if( key == 'h' ){
		if( panel.hidden ) panel.show();
		else panel.hide();
	}
	
	if( key == 'u' ){
		bDoUnload = true;
		currentMsg = "unloading with key press";
	}
	
	if( key == 'l' ){
		SP.loadDirectory("input/otherTest");
		currentMsg = "start loading with key press";		
	}
	
	if (key == ' '){
		bTogglePlayer = !bTogglePlayer;
	}
	ofxQuaternion rot;
	if(key == OF_KEY_DOWN) {
		rot.makeRotate(-.1, xunit3f);
		connexionCamera.addRotation(rot);
	} else if(key == OF_KEY_UP) {
		rot.makeRotate(.1, xunit3f);
		connexionCamera.addRotation(rot);
	} else if(key == OF_KEY_LEFT) {
		rot.makeRotate(-.1, yunit3f);
		connexionCamera.addRotation(rot);
	} else if(key == OF_KEY_RIGHT) {
		rot.makeRotate(.1, yunit3f);
		connexionCamera.addRotation(rot);
	} else if(key == ',') {
		rot.makeRotate(-.1, zunit3f);
		connexionCamera.addRotation(rot);
	} else if(key == '.') {
		rot.makeRotate(.1, zunit3f);
		connexionCamera.addRotation(rot);
	} else if(key == 'n') {
		connexionCamera.moveZoom(100);
	} else if(key == 'm') {
		connexionCamera.moveZoom(-100);
	}
}

//----------------------------------------------------------------
void testApp::exit() {
	ofxConnexion::stop();
}

//----------------------------------------------------------------
bool testApp::beginParticleMoveToTarget(string mode){
	if( mode == "TAKE_TURNS" ){
	
		//printf("STARTING TAKE TURNS\n");
		ofxVec3f avg = Particle::avg;
		
		int count = 0;
		for(int k = 0; k < PS.particles.size(); k++){	
		
			//TODO: should have particle.isQueued() to do this check
			if( PS.particles[k].state == PARTICLE_TARGET || PS.particles[k].stateQueue.size() ){
				continue;
			}
			
			//we only want to do one line at a time
			if( count > frameW ){
				return false;
			}

			//lets set just a few at a time
			PS.particles[k].queueState(PARTICLE_TARGET,  0.0);
			count++;
		}
	}else{
		printf("testApp::beginParticleMoveToTarget - no mode set\n");
	}
	
	return true;
}

//----------------------------------------------------------------
void testApp::beginParticleBreakApart(string mode){
	
	if( mode == "EXPLODE" ){
		
		//printf("STARTING EXPLODE BREAK APART \n");
		
		ofxVec3f avg = Particle::avg;
		ofxVec3f delta;
		for(int k = 0; k < PS.particles.size(); k++){
			delta = PS.particles[k].position - avg;
			delta.z *= 0.1;
			delta.normalize();
			
			PS.particles[k].explodeForce = delta * ofRandom(0.08, 0.18);
			
			float tToExplode = ofGetElapsedTimef() + ofRandom(0.0, 0.25);
			
			//TODO: should make the times relative to each other - not abs time
			PS.particles[k].queueState(PARTICLE_EXPLODE, tToExplode + ((float)k / (float)PS.particles.size()) * 2.0);
			PS.particles[k].queueState(PARTICLE_FREE,  tToExplode + 0.9 + ((float)k / (float)PS.particles.size()) * 2.0);
			PS.particles[k].queueState(PARTICLE_FLOCKING,  tToExplode + 1.9 + ((float)k / (float)PS.particles.size()) * 2.0);
		}
		
	}else{
		printf("testApp::beginParticleMoveToTarget - no mode set\n");
	}
}

//----------------------------------------------------------------
void testApp::setParticlesFromFace(){
	
	if (SP.totalNumFrames > 0){
				
		unsigned char * pixels = SP.TSL.depthImageFrames[SP.currentFrame].getPixels();
		
		int index = 0;
		int rgbaIndex = 0;
		ofxVec4f pixelColor;
		
		float depthRange = 640; // +/-320
		float depthValues = 255;
		float overallScaling = .5;
		float depthScaling = (depthRange / depthValues) * overallScaling;
		for (int j = 0; j < frameH; j++){
			for (int i = 0; i < frameW; i++){
			
				float zposition = pixels[rgbaIndex + 3] * depthScaling;
				float xPos		= ofMap(i, 0, frameW, 0, 1024);
				float yPos		= ofMap(j, 0, frameH, 0, 768);
				
				PS.particles[index].targetPosition.set(i*3 - frameW*3/2, j*3-frameH*3/2, zposition*3);
				
				pixelColor.set( pixels[rgbaIndex + 0]/255.0, pixels[rgbaIndex + 1]/255.0, pixels[rgbaIndex + 2]/255.0, zposition == 0 ? 0.0 : 1.0);
				pixelColor *= 3.0;
				
				//PS.particles[index].color  *= 0.05;
				PS.particles[index].color   = 0.85 * PS.particles[index].color + pixelColor * 0.15;
				
				if( PS.particles[index].color.w < 0.01 ){	
					PS.particles[index].color.w		= 0.0;
					PS.particles[index].bVisible	= false;
				}else{
					PS.particles[index].bVisible = true;
				}
				
				rgbaIndex += 4;
				index++;
			}
		}
		
	}
}

//--------------------------------------------------------------------------
void testApp::updateFreeParticles(){

	//if needed 
	//	for(int k = 0; k < PS.particles.size(); k++ ){
	//		
	//	}

}

//--------------------------------------------------------------------------
void testApp::eventsIn(eventStruct &dataIn){
	if( dataIn.message == "DecodeStarted" ){
		bDoUnload = true;
		currentMsg = "osc - recieved DecodeStarted";		
	}
	else if( dataIn.message == "TxStarted" && dataIn.folder != ""){
		//bDoUnload = true;
		currentMsg = "osc - recieved TxStarted";		
	}
	else if( dataIn.message == "TxEnded" && dataIn.folder != "" ){
		lastFolder = userFolder+"INCOMING_SCANS/"+dataIn.folder;
		
		printf("opening via OSC - %s\n", lastFolder.c_str());
		SP.loadDirectory(lastFolder, panel.getValueB("bConvertToPng") );
		notifier.clearData();
		currentMsg = "osc - recieved TxEnded - loading scan" + dataIn.folder;				
	}
}

//--------------------------------------------------------------------------
void testApp::update() {
	
	notifier.update();
	
	appFps = ofGetFrameRate();

	//TODO: move control panel updates to seperate method?
	// CONTROL PANEL 
		panel.update();

		Particle::speed				= panel.getValueF("particle_speed");
		Particle::spread			= panel.getValueF("particle_spread");
		Particle::viscosity			= panel.getValueF("particle_viscosity");
		Particle::independence		= panel.getValueF("particle_independence");
		Particle::neighborhood		= panel.getValueF("particle_neighborhood");
		Particle::targetForce		= panel.getValueF("particle_targetForce");	
		Particle::noiseScaleInput	= panel.getValueF("noise_scale_input");
		Particle::noiseScaleOutput	= panel.getValueF("noise_scale_output");
	
	connexionCamera.positionMomentum = panel.getValueF("positionMomentum");
	connexionCamera.zoomMomentum = panel.getValueF("zoomMomentum");
	connexionCamera.rotationMomentum = panel.getValueF("rotationMomentum");
		
		pointBrightness = panel.getValueF("point_brightness");
		aberration		= panel.getValueF("aberration");
		aperture		= panel.getValueF("aperture");
			
		bTogglePlayer	= panel.getValueB("toggle_mode");
		
		panel.clearAllChanged();
		
		if( panel.hidden ){
			ofHideCursor();
		}else{
			ofShowCursor();
		}
		
	// END CONTROL PANEL 
	
	SP.update();
	
	if(bTogglePlayer) {
	
		PS.updateAll(1.4);
	
	} else {
	
		// IF PARTICLES ARE FREE AND NEW FACE HAS COME IN
		if( state == VIZAPP_PARTICLES_FREE ){
			if( SP.TSL.state == TH_STATE_LOADED ){
				state = VIZAPP_NEWFACE;
			}
		}
	
		//LETS TRANSITION INTO NEW FACE
		if( state == VIZAPP_NEWFACE ){							
			if( beginParticleMoveToTarget("TAKE_TURNS") ){
				state = VIZAPP_PARTICLES_FACE;
			}
			setParticlesFromFace();	
			updateFreeParticles();		
		}
		
		//UPDATE FACE
		if( state == VIZAPP_PARTICLES_FACE  ){
			setParticlesFromFace();
		}
		
		//UPDATE FREE
		if( state == VIZAPP_PARTICLES_FREE ){
			updateFreeParticles();
		}

		//TODO: this is a key press right now - should have it hooked into osc
		if( bDoUnload ){
			SP.TSL.unload();
			beginParticleBreakApart("EXPLODE");
			bDoUnload = false;		
			state = VIZAPP_PARTICLES_FREE;	
		}
				
		PS.updateAll(1.4);
	}
	
	PS.calculate();
	connexionCamera.update();	// zach: I calculate amount of movement here
	
	
	
	daitoPrintout();
}


//--------------------------------------------------------------------------
void testApp::daitoPrintout(){
	// just some data for daito: 
	
	printf("------------------------------------ \n");
	printf("camera rotation amount %f \n", connexionCamera.quaternionChangeAmount);
	ofxDaito::bang("quaternionChangeAmount",connexionCamera.quaternionChangeAmount);

	printf("camera zoom amount (is zero for now) %f \n", connexionCamera.zoomChangeAmount);
	ofxDaito::bang("zoomChangeAmount",connexionCamera.zoomChangeAmount);
	
	printf("average position (%f,%f,%f) \n", PS.avgPosition.x, PS.avgPosition.y, PS.avgPosition.z);
	ofxDaito::bang("avgPosition",PS.avgPosition);

	printf("std dev position (%f,%f,%f) \n", PS.stdDevPosition.x, PS.stdDevPosition.y, PS.stdDevPosition.z);
	ofxDaito::bang("stdDevPosition",PS.stdDevPosition);

	printf("std dev position length %f \n", PS.stdDevPosition.length());
	ofxDaito::bang("stdDevPositionLength",PS.stdDevPosition.length());
	
	printf("average velocity (%f,%f,%f) \n", PS.avgVelocity.x, PS.avgVelocity.y, PS.avgVelocity.z);
	ofxDaito::bang("avgVelocity",PS.avgVelocity);

	printf("std dev velocity (%f,%f,%f) \n", PS.stdDevVelocity.x, PS.stdDevVelocity.y, PS.stdDevVelocity.z);
	ofxDaito::bang("stdDevVelocity",PS.stdDevVelocity);

	printf("average velocity length %f \n", PS.avgVelocity.length());
	ofxDaito::bang("avgVelocityLength",PS.avgVelocity.length());

	printf("std dev velocity length %f \n", PS.stdDevVelocity.length());
	ofxDaito::bang("stdDevVelocityLength",PS.stdDevVelocity.length());
	
	ofxDaito::bang("cameraRotation", ofRadToDeg(connexionCamera.amount), connexionCamera.angle.x, connexionCamera.angle.y, connexionCamera.angle.z);
	printf("cameraRotation %f %f %f %f \n", ofRadToDeg(connexionCamera.amount), connexionCamera.angle.x, connexionCamera.angle.y, connexionCamera.angle.z);
	
		
}

//--------------------------------------------------------------------------
void testApp::draw() {
	
	//aberration = ofMap(mouseX, 0, ofGetWidth(), 0, 1);
	//pointBrightness = ofMap(mouseY, 0, ofGetHeight(), 0, 1);
	
	ofBackground(0, 0, 0);
	
	chroma.begin();
	chroma.setFov(panel.getValueF("fov"));
	
	if( ofGetFrameNum() < 20 || !panel.getValueB("do_trails") ){
		chroma.setBackground(0, 0, 0, 1);
	}else{
		ofSetColor(0, 0, 0, ofMap(connexionCamera.quaternionChangeAmount, 0,0.25, 255,100, true));	
		ofFill();
		ofRect(0, 0, ofGetWidth(), ofGetHeight());
	}

	ofPushMatrix();
	
	connexionCamera.draw(mouseX, mouseY);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	
	// super helpful: http://pmviewer.sourceforge.net/method.php
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);
	
	dofShader.begin();
	
	ofxVec3f& avg = Particle::avg;
	float distance = avg.distance(ofPoint(0, 0, connexionCamera.getZoom()));
	
	dofShader.setUniform("focusDistance", distance + panel.getValueF("focus_offset"));
	dofShader.setUniform("aperture", aperture);
	dofShader.setUniform("pointBrightness", pointBrightness);
	
	
	if (panel.getValueB("bDrawParticles")){
		PS.drawAll();
	}

	dofShader.end();
	
	float sphereSize = 10000;
	sphereShader.begin();
	glColor4f(1, 1, 1, panel.getValueF("sphere_alpha"));
	glutSolidSphere(sphereSize, 32, 16);
	sphereShader.end();
	
	ofPopMatrix();
	
	chroma.end();
	chroma.clearAlpha();
	
	drawWithAberration();
	
	SP.draw();
	
	if( !panel.hidden ){
		ofSetColor(255, 255, 255, 255);
		ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
		
		panel.draw();
		ofDrawBitmapString("keys: [u]nload - [l]oad", 340, 20);
		
		ofDrawBitmapString("currentMsg: "+currentMsg, 10, ofGetHeight()-10);
	}
}

//--------------------------------------------------------------------------
void testApp::drawWithoutAberration() {
	chroma.draw(0, 0);
}

//--------------------------------------------------------------------------
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

//--------------------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	if( !panel.mouseDragged(x, y, button) ){
		if( !bTogglePlayer ){
			Particle::viscosity		= ofMap(mouseX, 300, ofGetWidth(), 0.1, 1, true);
			Particle::targetForce	= ofMap(mouseX, ofGetWidth(), 0, 1, true);
		}
	}
}

//--------------------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	isMousePressed = true;
	panel.mousePressed(x, y, button);
}

//--------------------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	isMousePressed = false;
	panel.mouseReleased();	
}
