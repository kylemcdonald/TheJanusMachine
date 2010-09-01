/*
 *  threadedImageConvertor.h
 *  emptyExample
 *
 *  Created by itotaka on 1/13/10.
 *  Copyright 2010 YCAM. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxThread.h"
#include "ofxDirList.h"

class threadedImageConvertor : public ofxThread {
	
	public:
	
		threadedImageConvertor();	
		
		bool convertPNGToTGA(string folderPath, bool bDeleteAfter);
		void threadedFunction();
	
	protected:
		bool loadScans(string pathName);

		string	folderName;
		bool bDelete;
	
};





