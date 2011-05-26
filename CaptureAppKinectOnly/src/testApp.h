#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"
#include "ofxCv.h"
using namespace cv;
using namespace ofxCv;

#define FRAME_START_INDEX (100000)

class testApp : public ofBaseApp {
public:
	~testApp();

	void loadSettings();

	void setup();
	void setupKinect();
	void setupOsc();
	void setupArduino();
	
	void update();
	void updateOsc();
	void updateArduino();
	void updateState();
	
	void checkKinect();
	
	void sendOsc(string msg, string dir = "", string timestamp = "", int imageCount = 0);
	void startFadeIn();
	void startRecord();
	void startFadeOut();
	
	void draw();
	void exit();
	void buttonPressed();
	void keyPressed(int key);
	
	ofxKinect kinect;
	
	Mat homography;
	float fovMultiplier;
	
	bool recording;
	int currentFrame;
	int captureFrameCount;
	float captureFrameInterval;
	vector<ofPixels*> depthBuffer;
	vector<ofPixels*> colorBuffer;
	vector<ofPixels*> rectifiedBuffer;
	
	static const int rectifiedWidth = 640;
	static const int rectifiedHeight = 480;
	
	ofSerial arduino;
	bool arduinoReady;
	string serialPort;
	int serialBaud;
	unsigned char curArduinoByte;
	
	ofxOscSender osc;
	string oscIp;
	int oscPort;
	
	float nearClipping, farClipping;
	
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
	int recordingState;
	State state, previousState;

	string currentTimestamp;
	string currentDecodeFolder;
	int totalFrameCount;
};
