#include "ofMain.h"
#include "ofApp.h"


string Param::ParamFilePath="_param.xml";
Param *Param::_instance=0;


float SceneBase::WinScale=1;
ofSoundPlayer SceneBase::SoundButton=ofSoundPlayer();
float SceneBase::EaseDue=1500;
float SceneBase::EaseDelay=0;
float SceneBase::EaseDistort=0.2;
ofImage* SceneBase::ImgPillLeft=0;
ofImage* SceneBase::ImgPillRight=0;


ofxTrueTypeFontUC TextRunner::TextFont;
int TextRunner::TimePerText=200;
float TextRunner::CharWid=0;

//PKeyboard ofApp::_keyboard=PKeyboard();
ofColor ofApp::MainColor[4]={ofColor(219,83,16),ofColor(36,92,169),ofColor(249,249,250),ofColor(157,157,158)};

//========================================================================
int main( ){
	//ofSetupOpenGL(1280,720,OF_WINDOW);			// <-------- setup the GL context

	//// this kicks off the running of my app
	//// can be OF_WINDOW or OF_FULLSCREEN
	//// pass in width and height too:
	//ofRunApp(new ofApp());


	ofGLWindowSettings settings;
	settings.setGLVersion(2,1);
	settings.width=1280;
	settings.height=720;
	

	ofCreateWindow(settings);

	ofRunApp(new ofApp());
}
