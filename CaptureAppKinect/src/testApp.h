#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxEdsdk.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"

#define FRAME_START_INDEX (100000)

class testApp : public ofBaseApp {
public:
	void loadSettings();

	void setup();
	void setupKinect();
	void setupCanon();
	void setupOsc();
	void setupArduino();
	void setupRectifier();
	
	void update();
	void updateOsc();
	void updateArduino();
	void updateState();
	
	void sendOsc(string msg, string dir = "", string timestamp = "", int imageCount = 0);
	void startFadeIn();
	void startRecord();
	void startFadeOut();
	
	void draw();
	void exit();
	void buttonPressed();
	void keyPressed(int key);
	
	ofxKinect kinect;
	ofxEdsdk::Camera canon;
	
	bool recording;
	int currentFrame;
	int captureFrameCount;
	float captureFrameInterval;
	vector<ofPixels> kinectBuffer;
	vector<ofPixels> canonBuffer;
	vector<ofPixels> rectifiedBuffer;
	
	static const int rectifiedWidth = 640;
	static const int rectifiedHeight = 480;
	
	ofSerial arduino;
	bool arduinoReady;
	string serialPort;
	int serialBaud;
	
	ofxOscSender osc;
	string oscIp;
	int oscPort;
	
	string outputDirectory;
	string outputPrefix;
	
	unsigned long buttonPressedTime;
	
	enum State {
		IDLE,
		FADE_IN,
		RECORD,
		FADE_OUT
	};
	int fadeInTime, recordTime, fadeOutTime;
	float fadeState;
	State state, previousState;
};
