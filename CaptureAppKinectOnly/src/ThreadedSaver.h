#pragma once

#include "ofMain.h"

class ThreadedSaver : public ofThread {
public:
	ThreadedSaver(ofPixels* image, string filename) :
	image(image),
	filename(filename),
	done(false) {
	}
	bool getDone() {
		return done;
	}
protected:
	ofPixels* image;
	string filename;
	bool done;
	
	void threadedFunction() {
		ofSaveImage(*image, filename);
		done = true;
	}
};

class ThreadedSaverManager : public ofThread {
public:
	static const int maxThreads = 4;
	void setup(vector<ofPixels*> images, vector<string> filenames) {
		this->images = images;
		this->filenames = filenames;
		startThread();
	}
protected:
	vector<ofPixels*> images;
	vector<string> filenames;
	
	void threadedFunction() {
		vector<ThreadedSaver*> savers;
		savers.resize(filenames.size());
		for(int i = 0; i < savers.size(); i++) {
			savers[i] = new ThreadedSaver(images[i], filenames[i]);
		}
		while(true) {
			int totalDone = 0;
			int totalRunning = 0;
			for(int i = 0; i < savers.size(); i++) {
				if(savers[i]->isThreadRunning()) {
					totalRunning++;
				}
				if(savers[i]->getDone()) {
					totalDone++;
				}
			}
			if(totalDone == savers.size()) {
				break;
			}
			if(totalRunning < maxThreads) {
				for(int i = 0; i < savers.size(); i++) {
					ThreadedSaver& cur = *savers[i];
					if(!cur.isThreadRunning() && !cur.getDone()) {
						cur.startThread(true, false);
						break;
					}
				}
			}
			ofSleepMillis(1);
		}
	}
};