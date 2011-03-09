#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxVec3f.h"
#include "Boid.h"
#include "Boids.h"
#include <map>
#include "ofxControlPanel.h"
#include "audioAnalysis.h"

#define BUFFER_SIZE 512

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		ofSoundPlayer 		beat;

		float 				* fftSmoothed;
		// we will draw a rectangle, bouncing off the wall:
		float 				px, py, vx, vy;

		int nBandsToGet;
		float prevx, prevy;
	
		//boids
		Boids * boids;
		char mode;
		ofxVec3f target;
	
		//ofxSimpleGuiToo gui;
		//map<string,float> slider_vals;
	
		float				vel;
		
		ofxControlPanel		panel;
		bool				bShowPanel;
	
		//Audio Analysis
		//Runs audio through FFT, and saves output to magnitude, phase, power, freq arrays.
		//Click show in Panel to see settings
		void audioReceived (float * input, int bufferSize, int nChannels); 
		audioAnalysis	audio;
		
		float * left;
		float * right;
		int 	bufferCounter;

};

#endif

