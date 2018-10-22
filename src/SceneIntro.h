#pragma once
#ifndef SCENE_INTRO_H
#define SCENE_INTRO_H

#include "SceneBase.h"

class SceneIntro:public SceneBase{
	
	vector<TextRunner> _hint;
	int _index_hint;
public:
	SceneIntro(ofApp *set_):SceneBase(set_){
				
		setup();
		_order_scene=1;

		_hint.push_back(TextRunner(959,435,L"����O�A�̳��w���O�F�˻��H"));
		_hint.push_back(TextRunner(959,492,L"����O�A�[���O�F���@�˪��a��H"));
		_hint.push_back(TextRunner(959,549,L"�ڭ̴��ݵۧA�b�o�̡A"));
		_hint.push_back(TextRunner(959,606,L"�d�U�@�ǰO�СB�@�ǷPĲ�A�P�ۤv��ܡA"));
		_hint.push_back(TextRunner(959,663,L"�N�A���PĲ�O�s�b�O�F�a�I"));

		int len=0;
		for(auto& h:_hint){
			h.setFadeOut(false);
			len+=h.getTotalTime();
		}

		_timer_in[2]=FrameTimer(_timer_in[2].getDue(),EaseDue*1.5+len);

		_index_hint=0;
		
		//ofAddListener(SceneBase::sceneInFinish,this,&SceneIntro::onSceneInFinish);
		ofAddListener(_timer_in[0].finish_event,this,&SceneIntro::onObjInFinish);
		ofAddListener(_timer_in[2].finish_event,this,&SceneIntro::onTextInFinish);
	}
	void loadImage(){
		
		ofImage obj_;
		obj_.loadImage("ui/obj-02.png");
		_img_ui.push_back(obj_);


		ofImage text_;
		text_.loadImage("ui/empty.png");
		_img_ui.push_back(text_);

		
		ofImage button_;
		button_.loadImage("ui/button-02.png");
		_img_ui.push_back(button_);		

		_button.push_back(ofRectangle(1773,512,51,55));
		for(int i=0;i<_button.size();++i) _enable_button.push_back(false);

		_mlayer=3;
		_zindex.push_back(0);
		_zindex.push_back(1);
		_zindex.push_back(2);

	} 
	void drawLayer(int i){		
		
		if(i==1) for(auto& h:_hint) h.draw();
		else SceneBase::drawLayer(i);
	}
	void update(float dt_){
		SceneBase::update(dt_);
		_hint[_index_hint].update(dt_);
		if(_hint[_index_hint].finish()){
			if(_index_hint<_hint.size()-1){
				_index_hint++;
				_hint[_index_hint].restart();
			}
		}
		
	}

	void buttonEvent(int index){
		_ptr_app->setScene(ofApp::PStatus::PCOLOR);

	}

	void onObjInFinish(int &e){	
		if(e!=_order_scene) return;
		
		_hint[_index_hint].restart();
		
	}
	void onTextInFinish(int &e){
		for(auto& en:_enable_button) en=true;
	}

	void init(){
		SceneBase::init();
		for(auto& t:_hint) t.reset();
		_index_hint=0;
	}

};



#endif

