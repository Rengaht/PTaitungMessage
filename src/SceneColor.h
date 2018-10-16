#pragma once
#ifndef SCENE_COLOR_H
#define SCENE_COLOR_H

#include "SceneBase.h"

class SceneColor:public SceneBase{

	
	

public:
	
	SceneColor(ofApp *set_):SceneBase(set_){
		setup();
		_order_scene=2;

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
		


		/*_img_hint.loadImage("ui/content/01-1.png");
		_timer_hint=FrameTimer(1500);
*/

		_button.push_back(ofRectangle(1773,512,51,55));

		_button.push_back(ofRectangle(143,583,110,110));
		_button.push_back(ofRectangle(302,583,110,110));
		_button.push_back(ofRectangle(461,583,110,110));
		_button.push_back(ofRectangle(619,583,110,110));

	} 
	void draw(){
		SceneBase::draw();
		
	}
	void update(float dt_){
		SceneBase::update(dt_);

		
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

	void onSceneInFinish(int &e){	
		//if(e==_order_scene) _timer_hint.restart();
	}

	void setColor(int set_){
		_ptr_app->setSelectColor(set_);
	}
};



#endif

