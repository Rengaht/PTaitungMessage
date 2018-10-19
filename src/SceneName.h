#pragma once
#ifndef SCENE_NAME_H
#define SCENE_NAME_H

#include "SceneBase.h"
#include "PTextInput.h"

class SceneName:public SceneBase{
	
	PTextInput* _input_name;
public:
	SceneName(ofApp *set_):SceneBase(set_){
		setup();
		_order_scene=3;

		_input_name=new PTextInput(1029,674,56,_ptr_app->_keyboard);
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
		if(i==1){
			_input_name->draw();			
		}
		SceneBase::drawLayer(i);		
	}
	void drawDebugInfo(){	
		SceneBase::drawDebugInfo();
		_input_name->drawDebug();
	}
	void update(float dt_){
		SceneBase::update(dt_);
		_input_name->update(dt_);		
	}

	void buttonEvent(int index){
		_ptr_app->setScene(ofApp::PStatus::PRECORD);

	}

	void onSceneInFinish(int &e){	
		if(e!=_order_scene) return;
		for(auto& en:_enable_button) en=true;
		_input_name->setFocus(true);
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
	void init(){
		SceneBase::init();		
		_input_name->reset();

	}
	void end(){
		
		_ptr_app->setUserName(_input_name->getValue());
		SceneBase::end();
	}
	bool handleMousePressed(float mouse_x,float mouse_y){
		
		bool b=SceneBase::handleMousePressed(mouse_x,mouse_y);

		if(_input_name->mouseInside(_mouse_pos.x,_mouse_pos.y)){
			_ptr_app->_show_keyboard=true;
			b=true;
			_input_name->setFocus(true);
		}

		return b;

	}
};



#endif

