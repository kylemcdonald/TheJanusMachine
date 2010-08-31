
#include "scanPlayer.h"
#include "ofxVec3f.h"


void scanPlayer::setup(){
	maxNumFrames = 15 * 20; // 15 secs 2
	imageData.allocate(320,240, GL_RGB);
	currentFrame = 0;
	totalNumFrames = 0;
	TSL.setup(maxNumFrames, 320, 240);
}


void scanPlayer::update(){
	if (TSL.state == TH_STATE_JUST_LOADED) {
		currentFrame = 0;
		totalNumFrames = TSL.totalNumFrames;
		TSL.state = TH_STATE_LOADED;			// make sure, but maybe not needed.
	}
	if (TSL.state != TH_STATE_LOADED){
		totalNumFrames = 0;
	} else {
		totalNumFrames = TSL.totalNumFrames;
	}
	
	if (totalNumFrames > 0){
		currentFrame++;
		
		// this is because some scans sequences were missing chunks of frames (ie output2) 
		// so I just make sure that we really do have something on this frame. 
		
		//while (TSL.bLoaded[currentFrame] == false){
			//currentFrame++;
		///	currentFrame %= totalNumFrames;
		//}
		currentFrame %= totalNumFrames;
		
	}
	
}

void scanPlayer::draw(){
	
	if (TSL.state == TH_STATE_LOADED){
		if (totalNumFrames > 0){
			
			//unsigned char * pixels = TSL.depthImageFrames[currentFrame].getPixels();
//			for (int i = 0; i < 320; i+=10){
//				for (int j = 0; j < 240; j++){
//					ofSetColor(pixels[(j*320+i)*4], pixels[(j*320+i)*4], pixels[(j*320+i)*4]);
//					ofRect(i,j,10,10);
//				}
//			}
			//ofImage temp;
			//temp.clone(TSL.depthImageFrames[currentFrame]);
			//temp.draw(0,0);
		}
	}
}


void scanPlayer::loadDirectory(string pathName){
	TSL.start(pathName);
};
