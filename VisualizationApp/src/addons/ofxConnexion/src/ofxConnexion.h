#pragma once

#include "ofMain.h"
#include "3DconnexionClient/ConnexionClientAPI.h"

class ConnexionData {
public:
	short translation[3];
	short	rotation[3];
	short deviceId;
	short buttonState;
	
	ConnexionData() :
			deviceId(0),
			buttonState(0) {
		translation[0] = 0;
		translation[1] = 0;
		translation[2] = 0;
		rotation[0] = 0;
		rotation[1] = 0;
		rotation[2] = 0;
	}
	
	bool getButton(int button);
	string toString() {
		stringstream ss;
		ss << "{" <<translation[0] << ", " << translation[1] << ", " <<translation[2] << "}/" <<
			"{" << rotation[0] << " " << rotation[1] << " " << rotation[2] << "}";
		return ss.str();
	}
};

class ofxConnexion {
public:
	static ofEvent<ConnexionData> connexionEvent;
	static ConnexionData connexionData;

	static void start(string appName);
	static void stop();
	static void setLed(bool state);

protected:	
	static string appName;
	static UInt16 clientId;
	static OSStatus threadedFunction(void* 	args);
	static void driverHandler(io_connect_t connection, natural_t messageType, void *messageArgument);
};
