#pragma once
#ifndef SCENE_INTRO_H
#define SCENE_INTRO_H

#include "SceneBase.h"

class SceneIntro:public SceneBase{
	
public:
	SceneIntro(ofApp *set_):SceneBase(set_){
				
		setup();
		_order_scene=1;


		//ofAddListener(SceneBase::sceneInFinish,this,&SceneIntro::onSceneInFinish);

	}
	void loadImage(){
		
		ofImage obj_;
		obj_.loadImage("ui/obj-02.png");
		_img_ui.push_back(obj_);


		ofImage text_;
		text_.loadImage("ui/text-02.png");
		_img_ui.push_back(text_);

		
		ofImage button_;
		button_.loadImage("ui/button-02.png");
		_img_ui.push_back(button_);


		/*_img_hint.loadImage("ui/content/01-1.png");
		_timer_hint=FrameTimer(1500);
*/

		_button.push_back(ofRectangle(1773,512,51,55));
	} 
	void draw(){
		SceneBase::draw();
	}
	void update(float dt_){
		SceneBase::update(dt_);

		
	}

	void buttonEvent(int index){
		_ptr_app->setScene(ofApp::PStatus::PCOLOR);

	}

	void onSceneInFinish(int &e){	
		//if(e==_order_scene) _timer_hint.restart();
	}


};



#endif

