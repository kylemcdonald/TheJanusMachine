#pragma once

#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "ofxVec3f.h"
#include "ofxQuaternion.h"

/*
 At first, call ofxDaito::setup("setting.xml").
 To trigger a sound event, call:
 ofxDaito::bang(eventName, energy, pan)
 energy is 0.0f - 1.0f, by default 1.0f
 pan is 0.0f - 1.0f, by defualt 0.5f
 */

class ofxDaito {
private:
	static ofxXmlSettings xml;
	static vector<ofxOscSender> senders;
	static void send(ofxOscMessage& msg);
	
public:
	static void setup(string settings, bool verbose = true);
//	static void bang(string eventName, float energy = 1.0f, float pan = 0.5f);
	static void bang(string eventName);
	static void bang(string eventName,float val);
	static void bang(string eventName,ofxVec3f _v);
//	static void bang(string eventName,ofxQuaternion _v);	
	static void bang(string eventNambe, float val0, float val1, float val2, float val3);
	static void sendCustom(ofxOscMessage &msg);
};

