#include "ofMain.h"
#include "testApp.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, testApp::targetWidth, testApp::targetHeight, OF_WINDOW);
	ofRunApp(new testApp());
}
