#include "ofApp.h"

#define DEF_RADIUS 200
#define SCALE_FFT 200
#define SPHERE_RESOLUTION 6
//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAlphaBlending();
	ofEnableDepthTest();

	mSphere.setResolution(SPHERE_RESOLUTION);
	mInnerRadius = DEF_RADIUS;
	mSphere.setRadius(DEF_RADIUS);
	mBands = mSphere.getMesh().getNumIndices() / 3;
	mSound.load("sound/timebend.mp3");
	mSound.play();
	mSound.setLoop(true);
	cout << mBands << endl;
	mShader.setGeometryInputType(GL_TRIANGLES);
	mShader.setGeometryOutputType(GL_TRIANGLE_STRIP);
	mShader.setGeometryOutputCount( 3 * 5); //4–Êo—Í
	if (ofIsGLProgrammableRenderer()) { mShader.load("shaders/shadersGL3/vert.glsl", "shaders/shadersGL3/frag.glsl", "shaders/shadersGL3/geom.glsl" ); }
	else { mShader.load("shaders/shadersGL2/vert.glsl", "shaders/shadersGL2/frag.glsl", "shaders/shadersGL2/geom.glsl"); }

}

//--------------------------------------------------------------
void ofApp::update(){
	//get fft
	float* fft = ofSoundGetSpectrum(mBands);
	if (enableSound) {

		for (int i = 0; i < mBands ; i++) {
			mFFT[i] = fft[i ] ;
		}
		for (int i = 0; i < mBands; i++) {
			mNormalizedFFT[i] = 255  * mFFT[i] ;
			ofVec3f rgb= convertHtoRGB(mNormalizedFFT[i]);
			mFFT2RGB[ i * 3 + 0] = rgb.x;
			mFFT2RGB[ i * 3 + 1] = rgb.y;
			mFFT2RGB[ i * 3 + 2] = rgb.z;
		}
	}


}

//--------------------------------------------------------------
void ofApp::draw(){
	static ofVec3f rotate = ofVec3f::zero();
	static ofVec3f preVal = ofVec3f::zero();
	float* val = ofSoundGetSpectrum(2);
	//cout << val[0] << " " << val[1] << endl;
	rotate.z += 0.1;
	int vec = val[0] - preVal.x > 0 ? 1 : -1;
	rotate.x = vec * 10;
	rotate.y = vec * 10;


	preVal.x = val[0];
	preVal.y = val[1];

	ofTranslate(ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.5);
	ofRotateZ(rotate.z);
	//ofRotateY(rotate.y);
	//ofRotateX(rotate.x);

	//cam.begin();

	mShader.begin();
	mShader.setUniform1iv("fft", mNormalizedFFT, mFFT.size());
	mShader.setUniform3fv("rgb", mFFT2RGB, mFFT.size());

	mSphere.draw();
	mShader.end();
	//cam.end ();

	float theta = 2 * PI / mBands;
	for (int i = 0; i < mBands; i++) {
		float rad = theta * i;
		float outerRadius = mInnerRadius + mFFT[i];
		//ofDrawLine(mInnerRadius * cos(rad), mInnerRadius * sin(rad) , outerRadius * cos(rad), outerRadius * sin(rad));
	}
	ofTranslate(-ofGetWindowWidth() * 0.5, -ofGetWindowHeight() * 0.5);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 's') {
		enableSound = !enableSound;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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


//-------------------------------
ofVec3f  ofApp::convertHtoRGB(float _h) {
	ofVec3f res; 

	float s = ofMap(_h , 0, 255, 0, 100) / 100;
	float v = 100;

	float r =255, g =255, b =255;
	if (s == 0) {
		res = ofVec3f(r, g, b) / 255;
		return res;
	}
	s = 1;
	int hi = (int)(floor((double)_h / 60)) % 6;
	float f = (float)_h / 60 - (float)hi;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f)*s);
	switch (hi) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	case 5:
		r = v;
		g = p;
		b = q;
		break;
	default:
		break;
	}

    res = ofVec3f(r, g, b) / 255;

	return res;

}
