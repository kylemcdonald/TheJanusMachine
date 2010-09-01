/*
 *  scanNotifyer.h
 *  emptyExample
 *
 *  Created by theo on 24/06/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once 

#include "ofxOsc.h"
#include "ofxXmlSettings.h"

typedef struct{
	string message;
	string folder;
	string ts;
	int numImages;
}eventStruct;

class scanNotifier{
	public:
	
	scanNotifier(){
		bOscSetup  = false;
		bOscEnable = false;
	}

	void setup( string xmlSettingsFile ){
		int port = 70007;
		
		ofxXmlSettings xml;
		if( xml.loadFile(xmlSettingsFile) ){
			port = xml.getValue("osc_port", port);
		}
		
		bOscSetup = true;
		rx.setup( port );
	}
	
	void clearData(){
		message		= "";
		folder		= "";
		timeStamp	= "";
		numImages	= 0;
	}
	
	void enable(){
		bOscEnable = true;
	}
	
	void disable(){
		bOscEnable = false;
	}
	
	void setEnabled( bool bEnabled ){
		bOscEnable = bEnabled;
	}
	
	bool update(){
		
		bool bHave = false;
	
		if( bOscEnable && bOscSetup ){

			ofxOscMessage m; 			
			while( rx.hasWaitingMessages() ){
				rx.getNextMessage(&m);
				bHave = true;
			}
			
			if( bHave ){
				message		= m.getArgAsString(0);
				folder		= m.getArgAsString(1);
				timeStamp	= m.getArgAsString(2);
				numImages	= m.getArgAsInt32(3);
				
				eventStruct data;
				data.message	= message;			
				data.folder		= folder;			
				data.ts			= timeStamp;			
				data.numImages	= numImages;			
				
				printf("firing event - message is %s %s %s - %i \n", message.c_str(), folder.c_str(), timeStamp.c_str(), numImages);
				ofNotifyEvent(theEvent, data, this);
			}
			
		}
		
		return bHave;
	}
	
	int numImages;
	string message;
	string folder;
	string timeStamp;
	
	ofEvent <eventStruct> theEvent;		
	
	ofxOscReceiver rx;

	bool bOscSetup;
	bool bOscEnable;
};