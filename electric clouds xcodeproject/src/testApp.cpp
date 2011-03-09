#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	// 0 output channels, 
	// 2 input channels
	// 44100 samples per second
	// BUFFER_SIZE samples per buffer
	// 4 num buffers (latency)
	
	ofSoundStreamSetup(0,2,this, 44100, BUFFER_SIZE, 4);
	
	left = new float[BUFFER_SIZE];
	right = new float[BUFFER_SIZE];
	
	// panel settings // 
	bShowPanel=true;
	panel.setup("Settings", 720, 0, 300, 748);
	
	audio.setup(&panel, left);
	
	vel = 0;
	
	ofSetFrameRate(60);
	boids = new Boids(50);
	
	//Load Panel Settings
	panel.loadSettings("cvSettings.xml");
	
	ofBackground(0, 0, 0);
}


//--------------------------------------------------------------
void testApp::update(){
	
	audio.update();
	
	//do boids
	boids->setMaxVelocity(vel);
	boids->flock();
	if (mode == 'a') {
		boids->arrive(target);
	}
	else if (mode == 'f') {
		boids->flee(target);
	}
	else if (mode == 'e') {
		boids->explode();
		mode = 0;
	}
	boids->update();
	boids->bounce();
	
	
	//sound stuff
	/*float * val = ofSoundGetSpectrum(nBandsToGet);		// request 128 values for fft
	for (int i = 0;i < nBandsToGet; i++){
		
		// let the smoothed calue sink to zero:
		fftSmoothed[i] *= 0.96f;
		
		// take the max, either the smoothed or the incoming:
		if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
		
	}*/
	
	//Update panel
	if(bShowPanel) panel.update();


}

//--------------------------------------------------------------
void testApp::draw(){
	
	float freqSum;

	// fft stuff
	for (int i = 0; i < audio.FFTanalyzer.nAverages; i++) {
		//vel = (float) audio.freq[i];
		freqSum += audio.freq[i];
	}
	
	//cout << "frequency sum = " << freqSum << endl;
	
	vel = (float) freqSum/audio.FFTanalyzer.nAverages;
	
	if (freqSum < 50.0) {
		mode = 'a';
		ofSetColor(64, 18, 30, 150);
	}
	 
	if (freqSum > 50.0 && freqSum < 100.0) {
		mode = 'f';
		ofSetColor(166, 60, 109, 150);
	}
	 
	if (freqSum > 100.0 && freqSum < 150.0) {
		mode = 'e';
		ofSetColor(70, 87, 89, 150);
	}
	
	if (freqSum > 150.0 && freqSum < 200.0) {
		mode = 'a';
		ofSetColor(52, 133, 133, 150);
	}
	
	if (freqSum > 200.0 && freqSum < 250.0) {
		mode = 'f';
		ofSetColor(24, 62, 74, 150);
	}
	
	if (freqSum > 250.0 && freqSum < 300.0) {
		mode = 'e';
		ofSetColor(197, 251, 251, 150);
	}
	
	if (freqSum > 300.0) {
		mode = 'a';
		ofSetColor(70, 87, 89, 150);
	}

	//draw boids
	//gui.draw();
	boids->draw();
	
	// draw the fft resutls:
	//ofSetColor(ofRandom(0, 255),ofRandom(0, 255),ofRandom(0, 255),100);
	
	/*float width = (float)(5*128) / nBandsToGet;
	for (int i = 0;i < nBandsToGet; i++){
		// (we use negative height here, because we want to flip them
		// because the top corner is 0,0)
		ofRect(ofGetWidth()/2  - (sin(fftSmoothed[i] * 50000)),ofGetHeight()/2 - (fftSmoothed[i] * 5000),fftSmoothed[i] * 500,fftSmoothed[i] * 500);
	}*/
	
	//Settings
	//Draw Video and/or Audio Settings images
	if(panel.getValueB("SHOWEQ")) audio.draw();
	
	//Draw panel
	//if(bShowPanel) panel.draw();
	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	if (key == ' ') {
		//gui.toggleDraw();
	}
	else if (key == 'a') {
		target.set(ofGetWidth()/2, ofGetHeight()/2,0);
		mode = key;
	}
	else if (key == 'f') {
		target.set(ofGetWidth()/2, ofGetHeight()/2,0);
		mode = key;
	}
	else if (key == 'e') {
		mode = key;
	}
	else if (key == 'c') {
		mode = 0;
	}
	else if (key =='z') {
		vel -= 0.5;
	}
	else if (key == 'x') {
		vel += 0.5;
	}
	
	switch (key) {
		case 'p':
			bShowPanel=!bShowPanel;
			break;
		case 'r':
			audio.reset();
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	panel.mouseDragged(x,y,button);
}
 
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	panel.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	panel.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
//Audio Receiver
//-------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){	
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		right[i] = input[i*2+1];
	}
}

