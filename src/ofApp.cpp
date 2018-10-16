#include "ofApp.h"
#include "SceneHome.h"
#include "SceneIntro.h"
#include "SceneColor.h"
#include "SceneName.h"
#include "SceneRecord.h"
#include "SceneInfo.h"



//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofEnableSmoothing();

	_param=new Param();
	_now_millis=ofGetElapsedTimeMillis();

	SceneBase::WinScale=max(ofGetWidth()/1920.0,ofGetHeight()/1080.0);
	TextRunner::TextFont.loadFont("font/NotoSansCJKtc-Regular.otf",21);
	TextRunner::CharWid=TextRunner::TextFont.getFontSize()*1.4;

	_img_back.loadImage("ui/back.png");
	
	
	loadScene();
	/*_mode=PEMPTY;
	setScene(PStatus::PHOME);*/

	_mode_pre=PEMPTY;
	_mode=PHOME;
	_scene[_mode]->init();


	ofAddListener(SceneBase::sceneInFinish,this,&ofApp::onSceneInFinish);
	ofAddListener(SceneBase::sceneOutFinish,this,&ofApp::onSceneOutFinish);

}

//--------------------------------------------------------------
void ofApp::update(){

	float dt_=ofGetElapsedTimeMillis()-_now_millis;
	_now_millis+=dt_;

	if(_in_transition && _mode_pre!=PStatus::PEMPTY)_scene[_mode_pre]->update(dt_);
	
	_scene[_mode]->update(dt_);

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetBackgroundColor(255);
	ofEnableAlphaBlending();
	_img_back.draw(0,0,ofGetWidth(),ofGetHeight());



	if(_in_transition && _mode_pre!=PStatus::PEMPTY){
#ifdef DRAW_DEBUG
		_scene[_mode_pre]->drawScaled(true);
#else
		_scene[_mode_pre]->drawScaled(false);
#endif
	}
	


#ifdef DRAW_DEBUG
	_scene[_mode]->drawScaled(true);
	ofDrawBitmapString("fps= "+ofToString(ofGetFrameRate()),10,10);
#else
	_scene[_mode]->drawScaled(false);
#endif
	
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch(key){
		//case 'a':
		//	setScene(SceneMode((_mode+1)%5));
		//	break;
		case 'f':
			ofToggleFullscreen();
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	_scene[_mode]->handleMousePressed(x,y);
}


void ofApp::onSceneInFinish(int &e){	
	ofLog()<<"scene "<<e<<" in finish!";	
	_in_transition=false;
}
void ofApp::onSceneOutFinish(int &e){
	ofLog()<<"scene "<<e<<" out finish!";
	//if(e==_mode_pre) _scene[_mode_pre]->reset();
	
}


void ofApp::loadScene(){
	_scene[0]=new SceneHome(this);
	_scene[1]=new SceneIntro(this);
	_scene[2]=new SceneColor(this);
	_scene[3]=new SceneName(this);
	_scene[4]=new SceneRecord(this);
	_scene[5]=new SceneInfo(this);

}

void ofApp::setScene(PStatus set_){

	ofLog()<<"set scene: "<<set_;
	switch(set_){
		case PCOLOR:
			setSelectColor(2);
			break;	
	}


	_mode_pre=_mode;
	_mode=set_;

	if(_mode_pre!=PEMPTY) _scene[_mode_pre]->end();
	
	_scene[_mode]->init();


	_in_transition=true;

	
}

void ofApp::setSelectColor(int set_){
	_select_color=set_;
}
ofColor ofApp::getSelectColor(){
	return MainColor[_select_color];
}
