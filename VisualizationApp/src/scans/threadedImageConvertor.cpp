/*
 *  threadedImageConvertor.cpp
 *  emptyExample
 *
 *  Created by itotaka on 1/13/10.
 *  Copyright 2010 YCAM. All rights reserved.
 *
 */

#include "threadedImageConvertor.h"
#include "ofxFileHelper.h"

//--------------------------------------------------------------
threadedImageConvertor::threadedImageConvertor() {
	folderName	= "";
	bDelete		= false;
}

//--------------------------------------------------------------
bool threadedImageConvertor::convertPNGToTGA(string folderPath, bool bDeleteAfter){
	
	folderName	= folderPath;
	bDelete		= bDeleteAfter;
	
	if (!isThreadRunning() && (folderName != "")) {
		startThread(true, false);  // blocking, verbose
		return true;
	}else{
		return false;
	}
	
}


//--------------------------------------------------------------
bool threadedImageConvertor::loadScans(string pathName) {
	
	printf("loading and converting\n");
	
	ofDirectoryLister directory;
	directory.allowExt("tga");

		
	int howMany = directory.listDir(pathName); 
	//howMany		= ofClamp(howMany, 0, maxNumFrames);
	
	if( howMany <= 0 ){
		printf("no images found!\n");
		return false;
	}else{
		
		ofImage img;
		
		for (int i = 0; i <  howMany; i++){
						
			string tgaPath = directory.getPath(i);
			string pngPath = tgaPath.substr(0, tgaPath.length()-4) + ".png";
			
			img.setUseTexture(false);
			img.loadImage(tgaPath);
			img.saveImage(pngPath);
		}
		
	}
	
	if( bDelete ){
		for (int i = 0; i <  howMany; i++){
			printf( "deleting %s \n", directory.getPath(i).c_str() );
			ofxFileHelper::deleteFile(directory.getPath(i) );
		}
		bDelete = false;
	}
	
	stopThread();

	return true;
}

//--------------------------------------------------------------
void threadedImageConvertor::threadedFunction() {
	while (isThreadRunning() != 0){
		loadScans(folderName);
	}
}

//--------------------------------------------------------------

