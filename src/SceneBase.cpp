#include "SceneBase.h"
#include "ofApp.h"


ofEvent<int> SceneBase::sceneInFinish=ofEvent<int>();
ofEvent<int> SceneBase::sceneOutFinish=ofEvent<int>();


SceneBase::SceneBase(ofApp *set_ptr){

	_ptr_app=set_ptr;
}
void SceneBase::setup(){

	loadImage();

	_mlayer=_img_ui.size();
	setupTimer();
}
void SceneBase::setupTimer(){	
	
	for(int i=0;i<_mlayer;++i){
		_timer_in.push_back(FrameTimer(EaseDue*(1+EaseDistort*i),i*EaseDelay));
		_timer_out.push_back(FrameTimer(EaseDue*(1+EaseDistort*i),i*EaseDelay));
	}
	
}

SceneBase::~SceneBase(){

}
void SceneBase::drawScaled(bool debug_){

	ofPushMatrix();
	ofScale(WinScale,WinScale);
		draw();
		if(debug_) drawDebugInfo();
	ofPopMatrix();
}
void SceneBase::draw(){	
	
	for(int i=0;i<_mlayer;++i){
		
		ofPushStyle();
		//ofSetColor(255,255*_timer_in[i].eval()*(1-_timer_out[i].eval()));
		ofPushMatrix();
		ofTranslate(_img_ui[i].getWidth()*(1-_timer_in[i].valEaseInOut()-_timer_out[i].valEaseInOut()),0);
		
		//if(_order_scene>1 && i==1) ofSetColor(_ptr_app->getSelectColor());
			_img_ui[i].draw(0,0);
		ofPopMatrix();

		ofPopStyle();	
	}	

}
void SceneBase::drawDebugInfo(){	
	ofPushStyle();
	ofSetColor(0,255,0);
	ofNoFill();
	for(auto &r:_button){
		ofDrawRectangle(r);
	}

	ofPopStyle();
}

void SceneBase::init(){
	
	
	for(int i=0;i<_mlayer;++i){
		_timer_out[i].reset();
		_timer_in[i].restart();
	}
	_status=SceneStatus::Init;
	_trigger_in=false;
	_trigger_out=false;
}

void SceneBase::end(){

	for(int i=0;i<_mlayer;++i) _timer_out[i].restart();
	_status=SceneStatus::End;

}

void SceneBase::update(float dt_){

	bool fin_=true;
	switch(_status){
		case Init:
			for(int i=0;i<_mlayer;++i){
				_timer_in[i].update(dt_);
				if(!_timer_in[i].finish()) fin_=false;
			}
			if(fin_){
				_status=Due;
				if(!_trigger_in){
					ofNotifyEvent(sceneInFinish,_order_scene,this);
					_trigger_in=true;
				}
			}
			break;
		case End:
			for(int i=0;i<_mlayer;++i){
				_timer_out[i].update(dt_);
				if(_timer_out[i].finish()) fin_=false;
			}
			if(fin_){
				if(!_trigger_out){
					ofNotifyEvent(sceneOutFinish,_order_scene,this);		
					_trigger_out=true;
				}
			}
			break;
		case Due:

			break;
	}
}

void SceneBase::reset(){
	for(int i=0;i<_mlayer;++i){
		_timer_in[i].reset();
		_timer_out[i].reset();
	}
}

void SceneBase::handleMousePressed(float mouse_x,float mouse_y){
	
	if(_status!=SceneStatus::Due) return;

	
	float scalex=mouse_x/WinScale;
	float scaley=mouse_y/WinScale;

	_mouse_pos.x=scalex;
	_mouse_pos.y=scaley;


	int len=_button.size();
	for(int i=0;i<len;++i){
		if(_button[i].inside(scalex,scaley)){
			buttonEvent(i);
			SoundButton.play();	
		}
	}
}