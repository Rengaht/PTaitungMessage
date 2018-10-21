#pragma once
#ifndef SCENE_INFO_H
#define SCENE_INFO_H

#include "SceneBase.h"

class SceneInfo:public SceneBase{

	PTextInput* _input_phone;
	PTextInput* _input_email;
public:
	SceneInfo(ofApp *set_):SceneBase(set_){
		setup();
		_order_scene=5;
		_timer_in[2]=FrameTimer(_timer_in[2].getDue(),EaseDue*1.5);

		_input_email=new PTextInput(1029,593,24,_ptr_app->_keyboard,30,"font/GothamHTF-Book.ttf");
		_input_phone=new PTextInput(1029,653,24,_ptr_app->_keyboard,20,"font/GothamHTF-Book.ttf");

		ofAddListener(SceneBase::sceneInFinish,this,&SceneInfo::onSceneInFinish);
		ofAddListener(_input_email->_event_enter,this,&SceneInfo::onEmailEnter);
		ofAddListener(_input_phone->_event_enter,this,&SceneInfo::onPhoneEnter);
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

		_button.push_back(ofRectangle(1596,579,150,75));
		for(int i=0;i<_button.size();++i) _enable_button.push_back(false);

		_mlayer=3;

		_zindex.push_back(0);
		_zindex.push_back(1);
		_zindex.push_back(2);

	} 
	void drawLayer(int i){
		if(i==0) ofSetColor(_ptr_app->getSelectColor());	
		if(i==1){
			_input_phone->draw();
			_input_email->draw();
		}
		SceneBase::drawLayer(i);
	}
	void drawDebugInfo(){	
		SceneBase::drawDebugInfo();
		_input_phone->drawDebug();
		_input_email->drawDebug();
	}
	void update(float dt_){
		SceneBase::update(dt_);
		_input_phone->update(dt_);
		_input_email->update(dt_);
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
	void init(){
		SceneBase::init();		
		_input_phone->reset();
		_input_email->reset();
		_input_email->setFocus(true);
	}
	void end(){
		
		_ptr_app->setUserPhone(_input_phone->getValue());
		_ptr_app->setUserEmail(_input_email->getValue());		

		SceneBase::end();
	}
	bool handleMousePressed(float mouse_x,float mouse_y){

		bool b=SceneBase::handleMousePressed(mouse_x,mouse_y);

		if(_input_phone->mouseInside(_mouse_pos.x,_mouse_pos.y)){
			_ptr_app->_keyboard->setLanguage(PKeyboard::PLANGUAGE::NUMBER);
			_ptr_app->_show_keyboard=true;
			b=true;
			_input_phone->setFocus(true);
			_input_email->setFocus(false);
		}
		if(_input_email->mouseInside(_mouse_pos.x,_mouse_pos.y)){
			_ptr_app->_keyboard->setLanguage(PKeyboard::PLANGUAGE::EN);
			_ptr_app->_show_keyboard=true;
			b=true;
			_input_phone->setFocus(false);
			_input_email->setFocus(true);
		}
		return b;

	}
	void onEmailEnter(int& e){
		_input_email->setFocus(false);
		
		_ptr_app->_keyboard->setLanguage(PKeyboard::PLANGUAGE::NUMBER);
		_input_phone->setFocus(true);
	}
	void onPhoneEnter(int& e){
		_ptr_app->_show_keyboard=false;
	}

};



#endif

