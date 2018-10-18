#pragma once
#ifndef SCENE_INFO_H
#define SCENE_INFO_H

#include "SceneBase.h"

class SceneInfo:public SceneBase{

public:
	SceneInfo(ofApp *set_):SceneBase(set_){
		setup();
		_order_scene=5;
		_timer_in[2]=FrameTimer(_timer_in[2].getDue(),EaseDue*1.5);

		ofAddListener(SceneBase::sceneInFinish,this,&SceneInfo::onSceneInFinish);

	}
	void loadImage(){
		ofImage obj_;
		obj_.loadImage("ui/obj-06.png");
		_img_ui.push_back(obj_);

		ofImage back_;
		back_.loadImage("ui/title-06.png");
		_img_ui.push_back(back_);

		ofImage button_;
		button_.loadImage("ui/button-06.png");
		_img_ui.push_back(button_);

		_button.push_back(ofRectangle(1054,729,150,75));
		for(int i=0;i<_button.size();++i) _enable_button.push_back(false);

		_mlayer=3;

		_zindex.push_back(0);
		_zindex.push_back(1);
		_zindex.push_back(2);

	} 
	void drawLayer(int i){
		if(i==0) ofSetColor(_ptr_app->getSelectColor());	
		SceneBase::drawLayer(i);
	}
	void update(float dt_){
		SceneBase::update(dt_);

		
	}

	void buttonEvent(int index){
		_ptr_app->setScene(ofApp::PStatus::PHOME);

	}

	void onSceneInFinish(int &e){	
		if(e!=_order_scene) return;
		for(auto& en:_enable_button) en=true;				
	}

	void draw(){	

		for(auto& i:_zindex){

			ofPushStyle();
			if(i==_mlayer-1) ofSetColor(255,255*_timer_in[i].valEaseInOut()*(1-_timer_out[i].valEaseInOut()));		

			ofPushMatrix();			
			if(i!=_mlayer-1)
				ofTranslate(_img_ui[i].getWidth()*(1-_timer_in[i].valEaseInOut()-_timer_out[i].valEaseInOut()),0);
			
			if(i!=0 || _status!=Init) drawLayer(i);	

			ofPopMatrix();

			ofPopStyle();	
		}	

	}

};



#endif

