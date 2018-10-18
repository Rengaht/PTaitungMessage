#pragma once
#ifndef SCENE_RECORD_H
#define SCENE_RECORD_H

#define POS_PILL_LOPEN 0
#define POS_PILL_LCLOSE 344

#define POS_PILL_ROPEN 1304
#define POS_PILL_RCLOSE 960
#define POS_PILL_Y 170

#include "SceneBase.h"



class SceneRecord:public SceneBase{
	
	//bool _mode_record;
	ofImage _img_listen;
	ofImage _img_pill_left,_img_pill_right;

	FrameTimer _timer_pill,_timer_scale;
	FrameTimer _timer_record;

	float _pos_dest_left,_pos_begin_left;
	float _pos_dest_right,_pos_begin_right;
	
	float _dest_scale,_begin_scale;


	TextRunner _hint;

	ofxTrueTypeFontUC _font_time;

	FrameTimer _timer_button_in,_timer_button_out;
	FrameTimer _timer_time_in,_timer_time_out;
	
	FrameTimer _timer_sample;

	list<float> _volume_record;
	int _index_spectrum;

public:
	enum RMode {LANDING,WAIT,RECORD,PLAY,FINISH,CLOSE};
	RMode _mode;


	SceneRecord(ofApp *set_):SceneBase(set_){
		setup();
		_order_scene=4;
		setMode(RMode::WAIT);

		_timer_record=FrameTimer(_ptr_app->_param->_time_record);
		_timer_pill=FrameTimer(800);
		_timer_button_in=FrameTimer(400);
		_timer_button_out=FrameTimer(200);

		_timer_time_in=FrameTimer(500);
		_timer_time_out=FrameTimer(250);

		_timer_sample=FrameTimer(_ptr_app->_param->_time_record/(float)_ptr_app->_param->_spectrum_size);

		_begin_scale=1.0;
		_dest_scale=1.316;
		_timer_scale=FrameTimer(450);

		_font_time.loadFont("font/GothamHTF-Book.ttf",34);

		ofAddListener(SceneBase::sceneInFinish,this,&SceneRecord::onSceneInFinish);
		ofAddListener(_timer_pill.finish_event,this,&SceneRecord::onPillFinish);
		ofAddListener(_timer_button_in.finish_event,this,&SceneRecord::onButtonInFinish);
		ofAddListener(_timer_scale.finish_event,this,&SceneRecord::onScaleFinish);

	}
	void loadImage(){

		ofImage obj_;
		obj_.loadImage("ui/obj-03.png");
		_img_ui.push_back(obj_);


		ofImage button_;
		button_.loadImage("ui/button-03.png");
		_img_ui.push_back(button_);

		_img_listen.loadImage("ui/button-04.png");

		_img_pill_left.loadImage("ui/pill-23.png");
		_img_pill_right.loadImage("ui/pill-24.png");
	

		_button.push_back(ofRectangle(917,892,85,85));

		_button.push_back(ofRectangle(826,928,57,57));
		_button.push_back(ofRectangle(938,928,57,57));
		_button.push_back(ofRectangle(1026,928,57,57));

		for(int i=0;i<_button.size();++i) _enable_button.push_back(false);

		_mlayer=2;
		_zindex.push_back(0);
		_zindex.push_back(1);

	}
	void setHint(){
		auto str_=_ptr_app->_param->getRandomQuestion();
		_hint=TextRunner(227,str_);	
		
		//_hint.setCont(true);
	}

	void drawLayer(int i){
		
		float v=_timer_pill.valEaseInOut();
		float v2=ofLerp(_begin_scale,_dest_scale,_timer_scale.valEaseOut());

		float px1=ofLerp(_pos_begin_left,_pos_dest_left,v);
		float px2=ofLerp(_pos_begin_right,_pos_dest_right,v);
		float py=POS_PILL_Y;
		float cx=960;
		float cy=540;

		switch(i){			
			case 0: //pill

				drawSpectrum();

				ofSetColor(_ptr_app->getSelectColor());	

				ofPushMatrix();
				ofTranslate(px1,py);	
				ofTranslate(cx-px1,cy-py);
				ofScale(v2,v2);
				ofTranslate(-cx+px1,-cy+py);
					_img_pill_left.draw(0,0);
				ofPopMatrix();

				ofPushMatrix();
				ofTranslate(px2,py);	
				ofTranslate(cx-px2,cy-py);
				ofScale(v2,v2);
				ofTranslate(-cx+px2,-cy+py);
					_img_pill_right.draw(0,0);
				ofPopMatrix();
			
				

				break;
			case 1: //button
				ofPushStyle();
				ofSetColor(255,255*_timer_button_in.valEaseInOut()*(1-_timer_button_out.valEaseInOut()));
					
					if(_mode==RMode::WAIT) _img_ui[i].draw(0,0);
					else if(_mode==RMode::FINISH) _img_listen.draw(0,0);		

				ofPopStyle();

				float a=_timer_time_in.valEaseInOut()*(1-_timer_time_out.valEaseInOut());
				ofPushStyle();
				ofSetColor(255,255*a);
					drawTime();				
				ofPopStyle();

				
				_hint.draw(a);

				break;				
		}
				
		
	}
	

	void update(float dt_){
		SceneBase::update(dt_);
		_timer_pill.update(dt_);
		_timer_record.update(dt_);		
		
		_timer_button_in.update(dt_);
		_timer_button_out.update(dt_);

		_timer_time_in.update(dt_);
		_timer_time_out.update(dt_);

		_timer_scale.update(dt_);
		
		_hint.update(dt_);
		if(_hint.finish()){
			setHint();
			_hint.restart();
		}

		switch(_mode){		
			case RECORD:
			case PLAY:
				
				if(_timer_record.finish()) setMode(FINISH);
				break;
			case CLOSE:				
				break;
		}
		
		
		/*_timer_sample.update(dt_);
		if(_timer_sample.finish()){
			if(_mode==RECORD){
				_volume_record.push_back(_ptr_app->_volume_now);
				if(_volume_record.size()>Param::val()->_spectrum_size) _volume_record.pop_front();
			}
			_timer_sample.restart();
			if(_index_spectrum<_volume_record.size()-1) _index_spectrum++;
		}*/

		_timer_sample.update(dt_);
		if(_timer_sample.finish()){

			if(_mode==RECORD){

				int m=floor(Param::val()->_spectrum_size*_timer_record.val())-_volume_record.size();
				for(int i=0;i<m;++i) _volume_record.push_back(_ptr_app->_volume_now);
				if(_volume_record.size()>Param::val()->_spectrum_size) _volume_record.pop_front();
				_index_spectrum+=m;

			}else if(_mode==PLAY){
				_index_spectrum=floor(_timer_record.val()*_volume_record.size());				
			}
			_timer_sample.restart();
			_index_spectrum=ofClamp(_index_spectrum,0,_volume_record.size()-1);
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
				if(_mode==RMode::FINISH) setMode(CLOSE);
				break;
		}
	}
	void init(){
		SceneBase::init();		
		setHint();


		_pos_begin_left=_pos_dest_left=POS_PILL_LCLOSE;
		_pos_begin_right=_pos_dest_right=POS_PILL_RCLOSE;

		_timer_button_in.reset();
		_timer_button_out.reset();
		_timer_time_in.reset();
		_timer_time_out.reset();

		_timer_scale.reset();
	}

	void onSceneInFinish(int &e){	
		if(e!=_order_scene) return;

		_pos_begin_left=_pos_dest_left;
		_pos_begin_right=_pos_dest_right;
		_pos_dest_left=POS_PILL_LOPEN;	
		_pos_dest_right=POS_PILL_ROPEN;	
		
		_enable_button[0]=true;

		setMode(LANDING);
		
	}
	void onPillFinish(int &e){	
		if(_mode==LANDING){
			_timer_time_in.restart();			
			setMode(RMode::WAIT);

		}else if(_mode==CLOSE){
			_timer_button_out.restart();
			_timer_time_out.restart();
			
			_timer_scale.restart();
		}
	}
	void onScaleFinish(int &e){
		if(_mode==CLOSE){
			_ptr_app->setScene(ofApp::PStatus::PINFO);
		}
	}
	void onButtonInFinish(int &e){
		if(_mode==WAIT) _hint.restart();
	}
	
	void setMode(RMode set_){
		
		ofLog()<<"set record  mode= "<<set_;

		switch(set_){
			case LANDING:
				_timer_pill.restart();	
				break;
			case WAIT:				
				_timer_record.reset();
				_timer_button_out.reset();
				_timer_button_in.restart();	

				_volume_record.clear();
				_index_spectrum=0;
				break;
			case RECORD:
				_timer_record.restart();									
				_timer_button_out.restart();
								
				_ptr_app->setRecording(true);
				
				for(auto& en:_enable_button) en=false;
				
				_timer_sample.restart();
				break;
			case FINISH:				
				_timer_button_in.restart();
				_timer_button_out.reset();

				if(_mode==RECORD) _ptr_app->setRecording(false);

				_enable_button[1]=true;
				_enable_button[2]=true;
				_enable_button[3]=true;

				break;
			case PLAY:

				_index_spectrum=0;

				_timer_record.restart();
				_timer_button_out.restart();
				_ptr_app->playRecord();

				for(auto& en:_enable_button) en=false;

				break;
			case CLOSE:
				_pos_begin_left=_pos_dest_left;
				_pos_begin_right=_pos_dest_right;
				_pos_dest_left=POS_PILL_LCLOSE;	
				_pos_dest_right=POS_PILL_RCLOSE;	
				_timer_pill.restart();

				for(auto& en:_enable_button) en=false;

				break;
		
		}
		_mode=set_;
	}
	
	void drawTime(){
		ofPushStyle();
		//ofSetColor(255);

		
		int last_=_timer_record.getDue()*(1.0-_timer_record.val());
		string min_=ofToString(int(floor(last_/60000.0)),2,'0');
		string sec_=ofToString(int(floor((last_%60000)/1000.0)),2,'0');
		string mil_=ofToString(int(floor((last_%1000)/10.0)),2,'0');

	

		float wid_=_font_time.getFontSize()*.9;
		float x=0;

		ofPushMatrix();
		ofTranslate(960-wid_*(6+.6*2)/2,843);		
			for(int i=0;i<2;++i){
				_font_time.drawString(ofToString(min_[i]),x,0);
				x+=wid_;
			}
			x+=wid_*.1;
			_font_time.drawString(":",x,0);
			x+=wid_*.5;
			for(int i=0;i<2;++i){
				_font_time.drawString(ofToString(sec_[i]),x,0);
				x+=wid_;
			}
			x+=wid_*.1;
			_font_time.drawString(":",x,0);
			x+=wid_*.5;
			for(int i=0;i<2;++i){
				_font_time.drawString(ofToString(mil_[i]),x,0);
				x+=wid_;
			}
		ofPopMatrix();


		ofPopStyle();
	
	}
	void drawSpectrum(){
		float x1=616;
		float x2=1304;

		float wid=(x2-x1)/(_ptr_app->_param->_spectrum_size-1);
		float x=0;

		ofPushStyle();
		ofSetColor(255);
		ofNoFill();

		ofPushMatrix();
		ofTranslate(x1,540);				

		float v=_ptr_app->_volume_now;
		auto s=_volume_record.begin();

		switch(_mode){
			case LANDING:
			case WAIT:
				wid=(x2-x1)/(float)(FFT_NBANDS-1);
				ofBeginShape();
				ofVertex(0,0);
				for(int i=0;i<FFT_NBANDS;++i){
					//cout<<_ptr_app->_fft_band[i]<<" ";
					ofVertex(x,-ofClamp(_ptr_app->_fft_band[i]*100,-255,255));
					x+=wid;
				}
				//cout<<endl;
				//ofVertex(x2-x1,0);
				ofEndShape();
				break;
			case RECORD:
			case PLAY:
			case FINISH:
			case CLOSE:
				if(_index_spectrum>0){
					ofBeginShape();
					for(int i=0;i<_index_spectrum;++i,++s){
						float a=ofClamp(*s,-225,225);
						//ofDrawRectangle(x+wid*.1,0,wid*.8,a);
						ofVertex(x,a);
						ofVertex(x,-a);
						x+=wid;
					}
					ofEndShape();
				}
			break;
		}

		ofPopMatrix();

		ofPopStyle();
	}
	void draw(){	

		for(auto& i:_zindex){

			ofPushStyle();
			if(i==_mlayer-1) ofSetColor(255,255*_timer_in[i].valEaseInOut()*(1-_timer_out[i].valEaseInOut()));		
			
			ofPushMatrix();			
			if(i!=_mlayer-1){
				if(i==0 && _status==End){
					ofTranslate(960*(-_timer_out[i].valEaseInOut()),0);
				}else{
					ofTranslate(_img_ui[i].getWidth()*(1-_timer_in[i].valEaseInOut()-_timer_out[i].valEaseInOut()),0);
				}
			}
				drawLayer(i);
			ofPopMatrix();

			ofPopStyle();	
		}	

	}
};



#endif

