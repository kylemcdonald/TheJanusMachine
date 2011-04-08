#include "testApp.h"

void testApp::loadSettings() {
	ofxXmlSettings settings;
	settings.loadFile("settings.xml");
	
	settings.pushTag("serial");
	serialPort = settings.getValue("port", "");
	serialBaud = settings.getValue("baud", 0);
	settings.popTag();
	
	settings.pushTag("capture");
	captureFrameCount = settings.getValue("frameCount", 0);
	settings.popTag();
	
	settings.pushTag("timing");
	fadeInTime = settings.getValue("fadeIn", 0);
	recordTime = settings.getValue("record", 0);
	fadeOutTime = settings.getValue("fadeOut", 0);
	settings.popTag();
	
	settings.pushTag("transfer");		
	settings.pushTag("output");
	outputDirectory = settings.getValue("directory", "");
	outputPrefix =  settings.getValue("prefix", "");
	settings.popTag();
	
	settings.pushTag("osc");
	oscIp = settings.getValue("ip", "");
	oscPort = settings.getValue("port", 0);
	settings.popTag();
	settings.popTag();
}

void testApp::setup() {
	//ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(true);
	
	currentFrame = 0;
	recording = false;
	buttonPressedTime = 0;
	
	loadSettings();
	captureFrameInterval = recordTime / captureFrameCount;
		
	setupArduino();
	setupOsc();
	setupKinect();
	setupCanon();
	setupRectifier();
}

void testApp::setupKinect() {
	kinect.init();
	ofxKinectCalibration::setClippingInCentimeters(40, 110); 
	kinect.open();
	
	cout << "kinect is setup" << endl;
	
	if(kinect.isConnected()) {
		cout << "allocating " + ofToString(captureFrameCount) + " frames for kinect" << endl;
		kinectBuffer.resize(captureFrameCount);
		for(int i = 0; i < captureFrameCount; i++) {
			kinectBuffer[i].allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
		}
	}
}

void testApp::setupCanon() {
	canon.setup();
	
	cout << "canon is setup" << endl;
	
	if(canon.isConnected()) {
		cout << "allocating " + ofToString(captureFrameCount) + " frames for canon" << endl;
		canonBuffer.resize(captureFrameCount);
		for(int i = 0; i < captureFrameCount; i++) {
			canonBuffer[i].allocate(canon.getWidth(), canon.getHeight(), OF_IMAGE_COLOR);
		}
	}
}

void testApp::setupRectifier() {
	rectifiedBuffer.resize(captureFrameCount);
	for(int i = 0; i < captureFrameCount; i++) {
		rectifiedBuffer[i].allocate(rectifiedWidth, rectifiedHeight, OF_IMAGE_COLOR_ALPHA);
	}
}

void testApp::setupOsc() {
	osc.setup(oscIp, oscPort);
}

void testApp::setupArduino() {
	arduino.listDevices();
	arduinoReady = arduino.setup(serialPort, serialBaud);
	if(arduinoReady) {
		arduino.flush();
	}
}

void testApp::updateArduino() {
	if(arduinoReady) {
		if(arduino.available() > 0){
			char data[8];
			memset(data, 0, 8);
			arduino.readBytes((unsigned char*) data, 8);
			buttonPressed();
		}
		arduino.writeByte((unsigned char) (fadeState * 255));
	}
}

void testApp::updateState() {
	unsigned long curTime = ofGetSystemTime();
	unsigned long diff = curTime - buttonPressedTime;
	if(diff < fadeInTime) {
		state = FADE_IN;
		fadeState = ofMap(diff, 0, fadeInTime, 0, 1);
	} else if(diff < fadeInTime + recordTime) {
		state = RECORD;
		fadeState = 1;
	} else if(diff < fadeInTime + recordTime + fadeOutTime) {
		state = FADE_OUT;
		fadeState = ofMap(diff - fadeInTime - recordTime, 0, fadeOutTime, 1, 0);
	} else {
		state = IDLE;
		fadeState = 0;
	}
}

void testApp::sendOsc(string msg, string dir, string timestamp, int imageCount) {
	ofxOscMessage m;
	m.addStringArg(msg);
	m.addStringArg(dir);
	m.addStringArg(timestamp);
	m.addIntArg(imageCount);
	osc.sendMessage(m);
}


void testApp::startFadeIn() {
	sendOsc("FadeInStarted");
	currentFrame = 0;
}

void testApp::startRecord() {
	sendOsc("ScanStarted");
	recording = true;
}

void testApp::startFadeOut() {
	sendOsc("CaptureStopped");
	
	recording = false;
	
	stringstream systemTime;
	systemTime << ofGetSystemTime();
	string currentTimestamp = systemTime.str();
	string currentDecodeFolder = string("decoded-") + outputPrefix + "-" + currentTimestamp + "/";
	ofFileUtils::makeDirectory(outputDirectory + currentDecodeFolder, false, true);
	int totalFrameCount = currentFrame;
	
	// prepare/rectify images
	sendOsc("DecodeStarted");
	ofPixels canonRectified;
	canonRectified.allocate(rectifiedWidth, rectifiedHeight, OF_IMAGE_COLOR);
	canonRectified.set(255);
	for(int i = 0; i < totalFrameCount; i++) {		
		unsigned char* rgb = canonRectified.getPixels();
		unsigned char* a = kinectBuffer[i].getPixels();
		unsigned char* rgba = rectifiedBuffer[i].getPixels();
		int j = 0;
		int k = 0;
		int n = rectifiedWidth * rectifiedHeight;
		for(int m = 0; m < n; m++) {
			rgba[k++] = rgb[j++];
			rgba[k++] = rgb[j++];
			rgba[k++] = rgb[j++];
			rgba[k++] = a[m];
		}
	}
	sendOsc("DecodeEnded");
	
	// transfer images to disk -- this should happen in a more threaded way so everything can fade back
	sendOsc("TxStarted", currentDecodeFolder, currentTimestamp, totalFrameCount);
	for(int i = 0; i < totalFrameCount; i++) {
		string curFilename = outputDirectory + currentDecodeFolder + ofToString(FRAME_START_INDEX + i) + ".png";
		ofSaveImage(rectifiedBuffer[i], curFilename);
	}
	sendOsc("TxEnded", currentDecodeFolder, currentTimestamp, totalFrameCount);
}

void testApp::update() {
	ofBackground(100, 100, 100);
	
	updateArduino();
	updateState();
	
	if(state != previousState) {
		switch(state) {
			case FADE_IN: startFadeIn(); break;
			case RECORD: startRecord(); break;
			case FADE_OUT: startFadeOut(); break;
		}
	}
	previousState = state;
	
	canon.update();
	kinect.update();
	
	if(recording) {
		if(kinect.isFrameNew()) {// && canon.isFrameNew()) { // need to add the canon back in
			if(kinect.isConnected()) {
				kinectBuffer[currentFrame].setFromPixels(kinect.getDepthPixels(), kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
			}
			if(canon.isConnected()) {
				canonBuffer[currentFrame] = canon.getPixelsRef();
			}
			
			currentFrame++;
			
			if(currentFrame == captureFrameCount) {				
				recording = false;
			}
		}
	}
}

void testApp::draw() {
	ofBackground(0);
	
	ofSetColor(255);
	int cw = canon.getWidth() * (640. / 1056);
	int ch = canon.getHeight() * (640. / 1056);
	canon.draw(0, 0, cw, ch);
	kinect.drawDepth(0, ch);
	
	ofSetColor(ofColor::red);
	ofLine(320, 0, 320, ofGetHeight());
	ofLine(0, ch / 2, ofGetWidth(), ch / 2);
	ofLine(0, ch + 240, ofGetWidth(), ch + 240);
	
	ofSetColor(fadeState * 255);
	ofCircle(80, 80, 30, 30);
	float pulse = ofMap(sin(ofGetElapsedTimef() * 2), -1, 1, 0, 1);
	ofSetColor((1 - fadeState) * 255 * pulse);
	ofCircle(100, 100, 10, 10);
}

void testApp::exit() {
	kinect.close();
}

void testApp::buttonPressed() {
	if(state == IDLE) {
		buttonPressedTime = ofGetSystemTime();
	}
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		buttonPressed();
	}
}