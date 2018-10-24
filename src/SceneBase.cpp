#include "SceneBase.h"
#include "ofApp.h"


ofEvent<int> SceneBase::sceneInFinish=ofEvent<int>();
ofEvent<int> SceneBase::sceneOutFinish=ofEvent<int>();
FrameTimer SceneBase::_timer_sleep=FrameTimer();


SceneBase::SceneBase(ofApp *set_ptr){

	_ptr_app=set_ptr;
	_timer_sleep=FrameTimer(Param::val()->_time_sleep);
}
void SceneBase::setup(){

	loadImage();	
	setupTimer();
}
void SceneBase::setupTimer(){	

	for(int i=0;i<_mlayer;++i){
		if(i==_mlayer-1){
			_timer_in.push_back(FrameTimer(EaseDue*.5,i*EaseDelay+(EaseDue*1.5+EaseDue*EaseDistort*(i-2))));
			_timer_out.push_back(FrameTimer(EaseDue*.5));
		}else if(i==0){
			_timer_in.push_back(FrameTimer(EaseDue*(1+EaseDistort*(_mlayer-2)),EaseDue*.5+i*EaseDelay));
			_timer_out.push_back(FrameTimer(EaseDue*(1+EaseDistort*i),EaseDue*.5));
		}else{
			_timer_in.push_back(FrameTimer(EaseDue*(1+EaseDistort*i+EaseDistort*(_mlayer-2)),EaseDue*.5+i*EaseDelay));
			_timer_out.push_back(FrameTimer(EaseDue*(1+EaseDistort*i),EaseDue*.5));

		}
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
	

	//ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofPushStyle();
	//ofEnableAlphaBlending();


	for(auto& i:_zindex){
				
		ofPushStyle();
			ofPushMatrix();
			
			if(i==_mlayer-1) ofSetColor(255,255*_timer_in[i].valEaseInOut()*(1-_timer_out[i].valEaseInOut()));		
			else ofTranslate(_img_ui[i].getWidth()*(1-_timer_in[i].valEaseOut()-_timer_out[i].valEaseOut()),0);
				
				drawLayer(i);

			ofPopMatrix();
		ofPopStyle();	
	}	
	
	

	ofPopStyle();

}
void SceneBase::drawLayer(int i){
		_img_ui[i].draw(0,0);
}
void SceneBase::drawDebugInfo(){	

	if(_status!=SceneStatus::Due) return;

	ofPushStyle();
	ofSetColor(0,255,0);
	ofNoFill();
	int len=_button.size();
	for(int i=0;i<len;++i){
		if(_enable_button[i]) ofDrawRectangle(_button[i]);
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

	for(auto& en:_enable_button) en=false;
}

void SceneBase::end(){

	for(int i=0;i<_mlayer;++i) _timer_out[i].restart();
	_status=SceneStatus::End;

	for(auto& en:_enable_button) en=false;
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

					_timer_sleep.restart();
				}
			}
			break;
		case Due:
			_timer_sleep.update(dt_);
			if(_order_scene!=0 && _timer_sleep.finish()){
				ofLog()<<"Back to sleep...";
				_ptr_app->setScene(ofApp::PStatus::PHOME);
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
	}
}

void SceneBase::reset(){
	for(int i=0;i<_mlayer;++i){
		_timer_in[i].reset();
		_timer_out[i].reset();
	}
}

bool SceneBase::handleMousePressed(float mouse_x,float mouse_y){
	
	//if(_status!=SceneStatus::Due) return;

	bool b=false;

	float scalex=mouse_x/WinScale;
	float scaley=mouse_y/WinScale;

	_mouse_pos.x=scalex;
	_mouse_pos.y=scaley;

	int len=_button.size();
	for(int i=0;i<len;++i){
		
		if(!_enable_button[i]) continue;

		if(_button[i].inside(scalex,scaley)){
			buttonEvent(i);
			b=true;
			
			if(_order_scene==4 && i<2){
				///no soud when recording
			}else if(_order_scene==5){
				SoundFinish.play();	
			}else SoundButton.play();	
		}
	}
	
	return b;
}

void SceneBase::drawPillLeft(float x1,float hei,int index_,float a_){
	ofPushStyle();
	ofSetColor(255,255*a_);
	
	ofPushMatrix();
	ofTranslate(x1-hei*1.153,540-hei/2);
		ImgPillLeft[index_].draw(0,0,hei*1.153,hei);
	ofPopMatrix();

	ofPopStyle();
}
void SceneBase::drawPillRight(float x1,float hei,int index_,float a_){
	ofPushStyle();
	ofSetColor(255,255*a_);
	ofPushMatrix();
	ofTranslate(x1,540-hei/2);
		ImgPillRight[index_].draw(0,0,hei*1.153,hei);
	ofPopMatrix();
	ofPopStyle();
}