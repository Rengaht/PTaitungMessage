#pragma once
#ifndef SCENE_NAME_H
#define SCENE_NAME_H

#include "SceneBase.h"

class SceneName:public SceneBase{

public:
	SceneName(ofApp *set_):SceneBase(set_){
		setup();
		_order_scene=3;


		ofAddListener(SceneBase::sceneInFinish,this,&SceneName::onSceneInFinish);

	}
	void loadImage(){

		ofImage obj_;
		obj_.loadImage("ui/obj-07.png");
		_img_ui.push_back(obj_);
		
		ofImage back_;
		back_.loadImage("ui/title-07.png");
		_img_ui.push_back(back_);

		ofImage button_;
		button_.loadImage("ui/button-07.png");
		_img_ui.push_back(button_);
	

		_button.push_back(ofRectangle(1773,512,51,55));
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
		_ptr_app->setScene(ofApp::PStatus::PRECORD);

	}

	void onSceneInFinish(int &e){	
		if(e!=_order_scene) return;
		for(auto& en:_enable_button) en=true;
	}

	void setupTimer(){	

		for(int i=0;i<_mlayer;++i){
			if(i==_mlayer-1){
				_timer_in.push_back(FrameTimer(EaseDue*.5,i*EaseDelay+(EaseDue*1.5+EaseDue*EaseDistort*(i-2))));
				_timer_out.push_back(FrameTimer(EaseDue*.5));
			}else if(i==0){
				_timer_in.push_back(FrameTimer(EaseDue*(1+EaseDistort*(_mlayer+1-2)),EaseDue*.5+i*EaseDelay));
				_timer_out.push_back(FrameTimer(EaseDue*(1+EaseDistort*i),EaseDue*.5));
			}else{
				_timer_in.push_back(FrameTimer(EaseDue*(1+EaseDistort*i),EaseDue*.5+i*EaseDelay));
				_timer_out.push_back(FrameTimer(EaseDue*(1+EaseDistort*i),EaseDue*.5));

			}
		}

	}
};



#endif

