#pragma once
#ifndef SCENE_HOME_H
#define SCENE_HOME_H

#include "SceneBase.h"
#include "TextRunner.h"

class SceneHome:public SceneBase{

	TextRunner _hint;
	FrameTimer _timer_hint;

public:
	SceneHome(ofApp *set_):SceneBase(set_){
		
		setup();

		_hint=TextRunner(959,741,L"走過臺東，你或許聽過海浪陣陣的旋律");
		_hint.setCont(true);

		_timer_hint=FrameTimer(5000);

		_order_scene=0;
		ofAddListener(SceneBase::sceneInFinish,this,&SceneHome::onSceneInFinish);

	}
	void loadImage(){

		ofImage back_;
		back_.loadImage("ui/title-01.png");
		_img_ui.push_back(back_);

		//ofImage text_;
		//text_.loadImage("ui/text-01.png");
		//_img_ui.push_back(text_);

		ofImage obj_;
		obj_.loadImage("ui/obj-01.png");
		_img_ui.push_back(obj_);

		ofImage button_;
		button_.loadImage("ui/button-01.png");
		_img_ui.push_back(button_);

		/*_img_hint.loadImage("ui/content/01-1.png");
		_timer_hint=FrameTimer(1500);
*/

		_button.push_back(ofRectangle(1773,512,51,55));
	} 
	void draw(){
		SceneBase::draw();
		_hint.draw();
	}
	void update(float dt_){
		SceneBase::update(dt_);
		_hint.update(dt_);
		

	}

	void buttonEvent(int index){
		_ptr_app->setScene(ofApp::PStatus::PINTRO);

	}

	void onSceneInFinish(int &e){	
		if(e==_order_scene) _hint.restart();		
			
	}
	
	void init(){
		SceneBase::init();
		_hint.reset();
	}
	

};



#endif

