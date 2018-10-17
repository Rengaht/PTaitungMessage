#pragma once
#ifndef SCENE_COLOR_H
#define SCENE_COLOR_H

#include "SceneBase.h"

class SceneColor:public SceneBase{

	
	FrameTimer _timer_color;
	ofColor _color,_pre_color;
public:
	
	SceneColor(ofApp *set_):SceneBase(set_){
		setup();
		_order_scene=2;
		_timer_color=FrameTimer(400);

		//ofAddListener(SceneBase::sceneInFinish,this,&SceneColor::onSceneInFinish);

	}
	void loadImage(){
		
		ofImage back_;
		back_.loadImage("ui/title-05.png");
		_img_ui.push_back(back_);

		ofImage button_;
		button_.loadImage("ui/button-05.png");
		_img_ui.push_back(button_);
		
		ofImage obj_;
		obj_.loadImage("ui/obj-05.png");
		_img_ui.push_back(obj_);
		
		ofImage button2_;
		button2_.loadImage("ui/button-01.png");
		_img_ui.push_back(button2_);

		_button.push_back(ofRectangle(1773,512,51,55));

		_button.push_back(ofRectangle(143,583,110,110));
		_button.push_back(ofRectangle(302,583,110,110));
		_button.push_back(ofRectangle(461,583,110,110));
		_button.push_back(ofRectangle(619,583,110,110));

		_mlayer=4;
		_zindex.push_back(2);
		_zindex.push_back(0);
		_zindex.push_back(1);
		_zindex.push_back(3);

	} 
	void drawLayer(int i){		

		if(i==2){
			float v=_timer_color.valEaseInOut();
			ofSetColor(ofLerp(_pre_color.r,_color.r,v),
						ofLerp(_pre_color.g,_color.g,v),
						ofLerp(_pre_color.b,_color.b,v));		
		}
		SceneBase::drawLayer(i);		
	}
	void update(float dt_){
		SceneBase::update(dt_);
		_timer_color.update(dt_);
		
	}

	void buttonEvent(int index){
		switch(index){
			case 0:
				_ptr_app->setScene(ofApp::PStatus::PNAME);
				break;
			case 1:
			case 2:
			case 3:
			case 4:
				setColor(index-1);
				break;
		}

	}
	void init(){
		SceneBase::init();		
		_ptr_app->setSelectColor(2);
		_pre_color=_color=_ptr_app->getSelectColor();
		_timer_color.reset();
	}
	void onSceneInFinish(int &e){	
		//if(e==_order_scene) _timer_hint.restart();
	}

	void setColor(int set_){		
		_ptr_app->setSelectColor(set_);

		_timer_color.restart();
		_pre_color=_color;
		_color=_ptr_app->getSelectColor();
	}
};



#endif

