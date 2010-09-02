
#pragma once

#include "ofMain.h"
//#include "ofx3DUtils.h"
#include "ofxDirList.h"
#include "threadedScanLoader.h"
//#include "Mesh.h"

#include "threadedImageConvertor.h"

class scanPlayer {

public:
	
	void setup(int w, int h);
	void loadDirectory(string pathName, bool bConvertAfterScan = false);
	
	ofImage *	depthFrames;
	ofImage *	imageFrames;
	bool	*	bLoaded;
	int			maxNumFrames;	// we should do this at some point
	
	int			totalNumFrames;
	int			currentFrame;
	
	ofTexture	imageData;
	
	threadedScanLoader TSL;
	bool bConvertAfter;
	
	void		update();
	void		draw();
	
	//Mesh		myMesh;
	//int addedId[320*240];
	
	string currentPath;
	
	threadedImageConvertor imageConvertor;
	
};
