#include "ofApp.h"
int n ;
//--------------------------------------------------------------
void ofApp::setup(){
    n = ofGetWidth()*ofGetHeight();
	ofSetLogLevel(OF_LOG_NOTICE);
#if defined(TARGET_LINUX_ARM)
	service.start("blinkytapeBridge", "_blinktape._tcp", 1234);
#elif defined(TARGET_OSX)
    bonjourServer.startService( "_blinktape._tcp.", "ecs", 7888 );
#endif
    //we run at 60 fps!
	ofSetVerticalSync(true);
    
	//setup the server to listen on 11999

    receiver.Create();
	receiver.Bind(11999);
	receiver.SetNonBlocking(true);
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    int baud = 115200;
	for(int i=0 ; i < deviceList.size() ; i++)
	{
#ifdef defined(TARGET_LINUX_ARM)
        if(deviceList[i].getDevicePath().find("/dev/ttyACM")!=string::npos)
#elif defined(TARGET_OSX)
        if(deviceList[i].getDevicePath().find("/dev/tty.usb")!=string::npos)
#endif
        {
            
            ofLogNotice("devicePath") << deviceList[i].getDevicePath();
            ofLogNotice("deviceName") << deviceList[i].getDeviceName();
            
            serial.setup(i, baud);
        }
	}
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
    unsigned char udpMessage[MAX_LENGTH];
	receiver.Receive((char*)udpMessage,MAX_LENGTH);

	if((char*)udpMessage != ""){
        serial.writeBytes(udpMessage,MAX_LENGTH);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    float i = ((float)x*y)/(float)n;
    bgColor.setHsb(i*360, 255 , 200);
    unsigned char d[3];
    d[0] = (bgColor.r>254)?254:(bgColor.r<1)?1:bgColor.r;
    d[1] = (bgColor.g>254)?254:(bgColor.g<1)?1:bgColor.g;
    d[2] = (bgColor.b>254)?254:(bgColor.b<1)?1:bgColor.b;
    for(int i = 0 ; i < MAX_LED ; i ++)
    {
        LED[i*3] = d[0];
        LED[i*3+1] = d[1];
        LED[i*3+2] = d[2];
    }

    LED[MAX_LENGTH-1] = 255;
    serial.writeBytes((unsigned char*)LED,MAX_LENGTH);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}