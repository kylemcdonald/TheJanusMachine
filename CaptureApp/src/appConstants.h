/*
 *  appConstants.h
 *  TDFCaptureApp
 *
 *  Created by theo on 21/06/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once 

#include "ofMain.h"

#define FRAME_START_INDEX (100000)

static string getTimeAsUniqueString(){
	string unique = ofToString(ofGetYear()) + ofToString(ofGetMonth()) + ofToString(ofGetDay()) + ofToString(ofGetHours()) + ofToString(ofGetMinutes()) + ofToString(ofGetSeconds());
	return unique;
}

static void transformSpaces(std::string & myStr){
        /* replace all spaces in the name to satisfy XML*/
        size_t found;
        found=myStr.find_first_of(" ");
        while (found!=string::npos)
        {
            //myStr.erase(found,1);
            myStr[found]='_';

            found=myStr.find_first_of(" ",found+1);
        }
}
