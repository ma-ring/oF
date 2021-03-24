#include "ofApp.h"

#define IP "127.0.0.1"
#define PORT 5000
#define VIDEO_FILE_PATH  "video.mp4"
#define MSG_BUFF_SIZE 100000
//--------------------------------------------------------------
void ofApp::setup(){
	
	mSender.Create();
	mSender.Connect(IP,PORT);
	mSender.SetNonBlocking(true);

	mReceiver.Create();
	mReceiver.Bind(PORT);
	mReceiver.SetNonBlocking(true);
	
	//mFrameImage.load("color.png");
	mTest.load("color.png");

	mVideoPlayer.load(VIDEO_FILE_PATH);
}

//--------------------------------------------------------------
void ofApp::update(){
	//send
	//if (mVideoPlayer.isLoaded()) {
		//mVideoPlayer.update();
		mTest.save("test.jpg",OF_IMAGE_QUALITY_WORST);
		ofBuffer buf = ofBufferFromFile("test.jpg");
		mSender.Send(buf.getData(), buf.size());
	//}
	
	//recieve
	char recvMsg[MSG_BUFF_SIZE];
	int recvSize = mReceiver.PeekReceive();
	if (recvSize > 0) {
		mReceiver.Receive(recvMsg, MSG_BUFF_SIZE);
		jpgEncoder(&recvMsg[0], recvSize, mFrameImage);
		
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	mFrameImage.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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

void ofApp::jpgDecoder(ofPixels src, char* dst, int& size) {

	FIMEMORY *hmemSrc, *hmemDst;
	FIBITMAP *bmp;

	hmemSrc = FreeImage_OpenMemory(&src.getData()[0], src.size());
	vector<unsigned char> data(dst, dst + size);
	hmemDst = FreeImage_OpenMemory(&data[0],  size);

	bmp = FreeImage_LoadFromMemory(FREE_IMAGE_FORMAT::FIF_RAW, hmemSrc, 0);
	FreeImage_SaveToMemory(FREE_IMAGE_FORMAT::FIF_JPEG, bmp, hmemDst);
}


void ofApp::jpgEncoder(char* src, int size, ofImage& dst) {
	vector<unsigned char> data(src, src + size);
	//mFrame = imdecode(data, 1);

	FIMEMORY *hmem;
	FREE_IMAGE_FORMAT fif;
	FIBITMAP *bmp;

	hmem = FreeImage_OpenMemory(&data[0], size);
	fif = FreeImage_GetFileTypeFromMemory(hmem, 0);
	bmp = FreeImage_LoadFromMemory(fif, hmem, 0);
	dst.setFromPixels(FreeImage_GetBits(bmp), 1920 / 4, 1080 / 4, ofImageType::OF_IMAGE_COLOR, 0);
	dst.update();
}