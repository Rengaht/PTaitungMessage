#pragma once
#ifndef SCENE_HOME_H
#define SCENE_HOME_H

#include "SceneBase.h"
#include "TextRunner.h"

class SceneHome:public SceneBase{

	TextRunner _hint;
	
public:
	SceneHome(ofApp *set_):SceneBase(set_){
		
		setup();

		_hint=TextRunner(959,741,L"走過臺東，你或許聽過海浪陣陣的旋律");
		_hint.setCont(true);

		_timer_in[0]=FrameTimer(EaseDue*(1+EaseDistort*3),EaseDue*(2+EaseDistort*2)+EaseDue*.5);
		_timer_in[1]=FrameTimer(EaseDue*(1+EaseDistort*2),EaseDue*(1+EaseDistort*2)+EaseDue*.5);
		_timer_in[2]=FrameTimer(EaseDue*.5,_timer_in[1].getDelay()+_timer_in[1].getDue()+_hint.getTotalTime());

		_order_scene=0;
		ofAddListener(_timer_in[0].finish_event,this,&SceneHome::onHintShoudBegin);
		ofAddListener(SceneBase::sceneInFinish,this,&SceneHome::onSceneInFinish);

	}
	void loadImage(){

		ofImage back_;
		back_.loadImage("ui/title-01.png");
		_img_ui.push_back(back_);

		ofImage obj_;
		obj_.loadImage("ui/obj-01.png");
		_img_ui.push_back(obj_);

		ofImage button_;
		button_.loadImage("ui/button-01.png");
		_img_ui.push_back(button_);

		_button.push_back(ofRectangle(1773,512,51,55));
		for(int i=0;i<_button.size();++i) _enable_button.push_back(false);


		_mlayer=3;
		_zindex.push_back(1);
		_zindex.push_back(0);
		_zindex.push_back(2);
	} 
	void draw(){	

		for(auto& i:_zindex){
			ofPushStyle();

			ofPushMatrix();			

			if(_status==SceneStatus::Init){
				if(i==2 || i==1) ofSetColor(255,255*_timer_in[i].valEaseInOut());		
				else ofTranslate(_img_ui[i].getWidth()*(-1+_timer_in[i].valEaseOut()),0);					
				
			}else{
				if(i==_mlayer-1) ofSetColor(255,255*(1-_timer_out[i].valEaseInOut()));		
				else ofTranslate(_img_ui[i].getWidth()*(-_timer_out[i].valEaseOut()),0);				
			
			}

			drawLayer(i);	

			ofPopMatrix();

			ofPopStyle();	
		}	

	}
	void drawLayer(int i){		
		SceneBase::drawLayer(i);
		if(i==0) _hint.draw();
	}

	void update(float dt_){
		SceneBase::update(dt_);
		_hint.update(dt_);
		

	}

	void buttonEvent(int index){
		_ptr_app->setScene(ofApp::PStatus::PINTRO);

	}

	void onSceneInFinish(int &e){	
		if(e==_order_scene){					
			for(auto& en:_enable_button) en=true;
		}			
	}
	void onHintShoudBegin(int &e){
		_hint.restart();
	}
	
	void init(){
		SceneBase::init();
		_hint.reset();
	}
	

};



#endif

