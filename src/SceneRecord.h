#pragma once
#ifndef SCENE_RECORD_H
#define SCENE_RECORD_H

#include "SceneBase.h"

class SceneRecord:public SceneBase{
	
	//bool _mode_record;
	ofImage _img_listen;
	ofImage _img_pill_left,_img_pill_right;

	FrameTimer _timer_pill;
	FrameTimer _timer_record;

	float _pos_dest_left,_pos_begin_left;
	float _pos_dest_right,_pos_begin_right;

	TextRunner _hint;

	ofxTrueTypeFontUC _font_time;

	FrameTimer _timer_button;

public:
	enum RMode {WAIT,RECORD,PLAY,FINISH};
	RMode _mode;


	SceneRecord(ofApp *set_):SceneBase(set_){
		setup();
		_order_scene=4;
		setMode(RMode::WAIT);

		_timer_record=FrameTimer(_ptr_app->_param->_time_record);
		_timer_pill=FrameTimer(800);
		_timer_button=FrameTimer(400);

		_font_time.loadFont("font/GothamHTF-Book.ttf",34);

		ofAddListener(SceneBase::sceneInFinish,this,&SceneRecord::onSceneInFinish);


	}
	void loadImage(){

		ofImage obj_;
		obj_.loadImage("ui/obj-03.png");
		_img_ui.push_back(obj_);

		ofImage back_;
		back_.loadImage("ui/title-03.png");
		_img_ui.push_back(back_);

		ofImage button_;
		button_.loadImage("ui/button-03.png");
		_img_ui.push_back(button_);

		_img_listen.loadImage("ui/button-04.png");

		_img_pill_left.loadImage("ui/pill-14.png");
		_img_pill_right.loadImage("ui/pill-22.png");

		/*_img_hint.loadImage("ui/content/01-1.png");
		_timer_hint=FrameTimer(1500);
*/

		_button.push_back(ofRectangle(917,892,85,85));

		_button.push_back(ofRectangle(826,928,57,57));
		_button.push_back(ofRectangle(938,928,57,57));
		_button.push_back(ofRectangle(1026,928,57,57));
	}
	void setHint(){
		auto str_=_ptr_app->_param->getRandomQuestion();
		_hint=TextRunner(227,str_);	
		_hint.setCont(true);
	}

	void draw(){
		
		float v=_timer_pill.valEaseInOut();

		for(int i=0;i<_mlayer;++i){

			ofPushStyle();
			ofPushMatrix();
			ofTranslate(_img_ui[i].getWidth()*(1-_timer_in[i].valEaseInOut()-_timer_out[i].valEaseInOut()),0);

				switch(i){
					case 1: //title
						_img_ui[i].draw(0,0);
						break;
					case 0: //pill
						_img_pill_left.draw(ofLerp(_pos_begin_left,_pos_dest_left,v),309,538,464);
						_img_pill_right.draw(ofLerp(_pos_begin_right,_pos_dest_right,v),309,538,464);
						break;
					case 2: //button
						if(_mode==RMode::WAIT) _img_ui[i].draw(0,0);
						else if(_mode==RMode::FINISH) _img_listen.draw(0,0);				
						break;				
				}
			ofPopMatrix();

			ofPopStyle();	
		}
				
		drawTime();


		/*if(!_mode_record) _hint.draw(1-_timer_pill.val());
		else */
		_hint.draw();
		
	}
	void update(float dt_){
		SceneBase::update(dt_);
		_timer_pill.update(dt_);
		_timer_record.update(dt_);		
		_hint.update(dt_);

		switch(_mode){
			case RECORD:
			case PLAY:
				if(_timer_record.finish()) setMode(FINISH);
				break;
		}
		
		
		
	}

	void buttonEvent(int index){
		switch(index){
			case 0:
				if(_mode==RMode::WAIT) setMode(RECORD);
				break;
			case 3:
				if(_mode==RMode::FINISH) setMode(WAIT);
				break;			
			case 1:
				if(_mode==RMode::FINISH) setMode(PLAY);
				break;
			case 2:
				if(_mode==RMode::FINISH){
				
					_ptr_app->setScene(ofApp::PStatus::PINFO);
					_pos_begin_left=_pos_dest_left;
					_pos_begin_right=_pos_dest_right;
					_pos_dest_left=432;	
					_pos_dest_right=960;	
					_timer_pill.restart();
				}
				break;
		}
	}
	void init(){
		SceneBase::init();		
		setHint();

		_pos_begin_left=_pos_dest_left=432;
		_pos_begin_right=_pos_dest_right=960;

		
	}

	void onSceneInFinish(int &e){	
		if(e==_order_scene) setMode(RMode::WAIT);
	}
	
	void setMode(RMode set_){
		
		ofLog()<<"set record  mode= "<<set_;

		switch(set_){
			case WAIT:
				_pos_begin_left=_pos_dest_left;
				_pos_begin_right=_pos_dest_right;
				_pos_dest_left=73;	
				_pos_dest_right=1310;	
				_timer_pill.restart();
				
				_timer_record.reset();
				_hint.restart();
				break;
			case RECORD:
				_timer_record.restart();									
				break;
			case FINISH:
				break;
			case PLAY:
				_timer_record.restart();									
				break;
		
		}
		_mode=set_;
		/*if(!_mode_record){
			_pos_begin_left=73;		_pos_dest_left=432;
			_pos_begin_right=1310;	_pos_dest_right=960;
			
			
		}else{
			_pos_dest_left=73;		_pos_begin_left=432;
			_pos_dest_right=1310;	_pos_begin_right=960;
			_hint.reset();

		}

		_timer_pill.restart();*/

		
	}
	
	void drawTime(){
		ofPushStyle();
		ofSetColor(255);

		
		int last_=_timer_record.getDue()*(1.0-_timer_record.val());
		string min_=ofToString(int(floor(last_/60000.0)),2,'0');
		string sec_=ofToString(int(floor((last_%60000)/1000.0)),2,'0');
		string mil_=ofToString(int(floor((last_%1000)/10.0)),2,'0');

		


		float wid_=_font_time.getFontSize()*.9;
		float x=0;

		ofPushMatrix();
		ofTranslate(960-wid_*(6+.14)/2,843);		
			for(int i=0;i<2;++i){
				_font_time.drawString(ofToString(min_[i]),x,0);
				x+=wid_;
			}
			x+=wid_*.3;
			_font_time.drawString(":",x,0);
			x+=wid_*.4;
			for(int i=0;i<2;++i){
				_font_time.drawString(ofToString(sec_[i]),x,0);
				x+=wid_;
			}
			x+=wid_*.3;
			_font_time.drawString(":",x,0);
			x+=wid_*.4;
			for(int i=0;i<2;++i){
				_font_time.drawString(ofToString(mil_[i]),x,0);
				x+=wid_;
			}
		ofPopMatrix();


		ofPopStyle();
	
	}
};



#endif
