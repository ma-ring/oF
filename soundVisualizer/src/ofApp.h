#pragma once

#include "ofMain.h"
#include "ofGraphics.h"
#define FFT_MAX_SIZE 200

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
private:
	
	ofEasyCam cam;

	ofShader mShader;
	ofSpherePrimitive mSphere;

	ofSoundPlayer mSound;
	int mBands;
	float mInnerRadius;
	std::map<int, float> mFFT;
	int mNormalizedFFT[FFT_MAX_SIZE];
	float mFFT2RGB[3 * FFT_MAX_SIZE];
	//ofVec3f mFFT2RGB[FFT_MAX_SIZE];

	bool enableSound = true;
	ofVec3f  convertHtoRGB(float h);
};