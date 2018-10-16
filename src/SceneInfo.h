#pragma once
#ifndef SCENE_INFO_H
#define SCENE_INFO_H

#include "SceneBase.h"

class SceneInfo:public SceneBase{

public:
	SceneInfo(ofApp *set_):SceneBase(set_){
		setup();
		_order_scene=5;


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


		/*_img_hint.loadImage("ui/content/01-1.png");
		_timer_hint=FrameTimer(1500);
*/

		_button.push_back(ofRectangle(1054,729,150,75));
	} 
	void draw(){
		SceneBase::draw();
	}
	void update(float dt_){
		SceneBase::update(dt_);

		
	}

	void buttonEvent(int index){
		_ptr_app->setScene(ofApp::PStatus::PHOME);

	}

	void onSceneInFinish(int &e){	
		//if(e==_order_scene) _timer_hint.restart();
	}


};



#endif

