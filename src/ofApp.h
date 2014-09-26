#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#if defined(TARGET_LINUX_ARM)
#include "ofxAvahiClientService.h"
#elif defined(TARGET_OSX)
#include "ofxBonjour.h"

using namespace ofxBonjour;

#endif

#define DELIM 1
#define MAX_LED (60)
#define MAX_LENGTH ((MAX_LED*3)+DELIM)
class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		bool isReady;
	
    ofxUDPManager receiver;
#if defined(TARGET_LINUX_ARM)
	ofxAvahiClientService service;
#elif defined(TARGET_OSX)
     Server bonjourServer;
#endif
	ofSerial	serial;
    ofColor bgColor;
    unsigned char LED[MAX_LENGTH];
};
