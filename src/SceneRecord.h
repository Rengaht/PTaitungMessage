#pragma once
#ifndef SCENE_RECORD_H
#define SCENE_RECORD_H

#define POS_PILL_LOPEN 610
#define POS_PILL_LCLOSE 960

#define POS_PILL_ROPEN 1310
#define POS_PILL_RCLOSE 960
//#define POS_PILL_Y 170
#define SPECTRUM_HEIGHT 200

#include "SceneBase.h"
#include "ofxPostProcessing.h"


class SceneRecord:public SceneBase{
	
	//bool _mode_record;
	ofImage _img_listen;
	//ofImage _img_pill_left,_img_pill_right;

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
	
	ofShader _shader_blurX,_shader_blurY;
	ofFbo _fbo1,_fbo2,_fbo_tmp;

	ofxPostProcessing post;
	
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

		_begin_scale=456;
		_dest_scale=600;
		_timer_scale=FrameTimer(450);

		_font_time.loadFont("font/GothamHTF-Book.ttf",34);

		ofAddListener(SceneBase::sceneInFinish,this,&SceneRecord::onSceneInFinish);
		ofAddListener(_timer_pill.finish_event,this,&SceneRecord::onPillFinish);
		ofAddListener(_timer_button_in.finish_event,this,&SceneRecord::onButtonInFinish);
		ofAddListener(_timer_scale.finish_event,this,&SceneRecord::onScaleFinish);


		
	}
	void loadImage(){

		ofImage obj_;
		obj_.loadImage("ui/S9dVuq4.png");
		_img_ui.push_back(obj_);


		ofImage button_;
		button_.loadImage("ui/button-03.png");
		_img_ui.push_back(button_);

		_img_listen.loadImage("ui/button-04.png");

		/*_img_pill_left.loadImage("ui/pill-23.png");
		_img_pill_right.loadImage("ui/pill-24.png");*/
	

		_button.push_back(ofRectangle(917,892,85,85));

		_button.push_back(ofRectangle(826,928,57,57));
		_button.push_back(ofRectangle(938,928,57,57));
		_button.push_back(ofRectangle(1026,928,57,57));

		for(int i=0;i<_button.size();++i) _enable_button.push_back(false);

		_mlayer=2;
		_zindex.push_back(0);
		_zindex.push_back(1);

		_shader_blurX.load("shader/gl2/shaderBlurX");
		_shader_blurX.begin();
		_shader_blurX.setUniform1f("windowWidth",1920);
		_shader_blurX.setUniform1f("windowHeight",1080);
		_shader_blurX.end();

		_shader_blurY.load("shader/gl2/shaderBlurY");
		_shader_blurY.begin();
		_shader_blurY.setUniform1f("windowWidth",1920);
		_shader_blurY.setUniform1f("windowHeight",1080);
		_shader_blurY.end();

		_fbo1.allocate(obj_.getWidth(),obj_.getHeight());
		_fbo2.allocate(obj_.getWidth(),obj_.getHeight());
		_fbo_tmp.allocate(obj_.getWidth(),obj_.getHeight());

		post.init(obj_.getWidth(),obj_.getHeight());
		post.createPass<BloomPass>()->setEnabled(true);

		
		

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

//		ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

		switch(i){			
			case 0: //pill

				//post.begin();
				_fbo_tmp.begin();
				ofClear(0);
				//_ptr_app->drawBackground();
				drawSpectrum();
				_fbo_tmp.end();
				drawGlow(_fbo_tmp);	

				ofPushStyle();
				
				_fbo_tmp.draw(0,0);
				/*
				ofDisableAlphaBlending();				
				ofEnableBlendMode(OF_BLENDMODE_ADD);
				_fbo2.draw(0,0);*/

				ofPopStyle();
				

				//post.end();

				drawPillLeft(px1,v2,_ptr_app->getSelectColor());
				drawPillRight(px2,v2,_ptr_app->getSelectColor());
				
				break;
			case 1: //button
				ofPushStyle();
				ofSetColor(255,255*_timer_button_in.valEaseInOut()*(1-_timer_button_out.valEaseInOut()));
					
					if(_mode==RMode::WAIT) _img_ui[i].draw(0,0);
					else if(_mode==RMode::FINISH) _img_listen.draw(0,0);		

				ofPopStyle();

				float a=_timer_time_in.valEaseInOut()*(1-_timer_time_out.valEaseOut());
				ofPushStyle();
				ofSetColor(255,255*a);
					drawTime();				
				ofPopStyle();

				
				_hint.draw(a);

				break;				
		}
				
		
	//	ofDisableBlendMode();

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
				_ptr_app->sendVolumeLight();
				if(_timer_record.finish()) setMode(FINISH);
				break;
			case WAIT:
				_ptr_app->sendVolumeLight();
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
				for(auto& en:_enable_button) en=false;
				_enable_button[0]=true;
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
				
				SceneBase::_timer_sleep.restart();

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
				_timer_button_out.restart();
				_timer_time_out.restart();

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

		//_ptr_app->drawBackground();
		float x1=616;
		float x2=1304;

		float wid=wid=(x2-x1)/(float)(FFT_NBANDS);//(x2-x1)/(_ptr_app->_param->_spectrum_size-1);
		

		ofPushStyle();
		ofSetColor(255);
		ofFill();
		

		//ofDrawRectangle(100,400,800,800);

		ofPushMatrix();
		ofTranslate(x1,540);				
		
		float v=_ptr_app->_volume_now;
		auto s=_volume_record.begin();
		
				//cout<<endl;
		switch(_mode){
			case LANDING:
			case CLOSE:	
			case FINISH:
				for(int i=0;i<FFT_NBANDS;++i){
					float a=SPECTRUM_HEIGHT*.01*ofRandom(1);
					float x=(x2-x1)/2.0-wid/2+(i%2==1?1:-1)*wid*floor(i/2);
					ofDrawRectangle(x+wid*.2,-abs(a),wid*.6,abs(a)*2);
					//x+=wid;
				}				
				break;
			case WAIT:	
			case RECORD:
				for(int i=0;i<FFT_NBANDS;++i){
					float a=SPECTRUM_HEIGHT*.01*ofRandom(1)+ofClamp(_ptr_app->eqOutput[i]*SPECTRUM_HEIGHT,0,SPECTRUM_HEIGHT);
					float x=(x2-x1)/2.0-wid/2+(i%2==1?1:-1)*wid*floor(i/2);
					ofDrawRectangle(x+wid*.2,-abs(a),wid*.6,abs(a)*2);
					//x+=wid;
				}
				
				break;
		
			case PLAY:
			
				float* tmp_=ofSoundGetSpectrum(FFT_NBANDS);

				for(int i=0;i<FFT_NBANDS;++i){
					//cout<<_ptr_app->_fft_band[i]<<" ";
					//float a=ofClamp(_ptr_app->_fft_band[i]*100,-SPECTRUM_HEIGHT,SPECTRUM_HEIGHT);
					float a=tmp_[i];

					a=SPECTRUM_HEIGHT*.01*ofRandom(1)+ofClamp(abs(a)*SPECTRUM_HEIGHT,0,SPECTRUM_HEIGHT);
					
					float x=(x2-x1)/2.0-wid/2+(i%2==1?1:-1)*wid*floor(i/2);
					ofDrawRectangle(x+wid*.2,-abs(a),wid*.6,abs(a)*2);
					x+=wid;
				}
				break;
		}

		ofPopMatrix();

		ofPopStyle();

		//post.end();

		//_img_ui[0].draw(0,0);

		
	}
	void drawGlow(ofFbo& src_){
		

		for(int i=0;i<1;++i){

			_fbo1.begin();

			ofDisableArbTex();

			ofClear(0,255);
			_shader_blurX.begin();
			src_.getTextureReference().bind();
			_shader_blurX.setUniform1f("blurAmnt",2.0);
				src_.draw(0,0);				
			src_.getTextureReference().unbind();
			_shader_blurX.end();

			_fbo1.end();

			_fbo2.begin();

			ofDisableArbTex();

			ofClear(0,255);
			_shader_blurY.begin();
			_fbo1.getTextureReference().bind();
			_shader_blurY.setUniform1f("blurAmnt",2.0);
				_fbo1.draw(0,0);
			_fbo1.getTextureReference().unbind();
			_shader_blurY.end();

			_fbo2.end();
		}
		//_fbo2.draw(0,0);		

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
		//ofPushStyle();
		//
		//ofSetColor(255,0,0);
		//ofDrawRectangle(1500,540,100,100);
		//ofSetColor(0,255,0);
		//ofDrawRectangle(1550,540,100,100);

		//ofSetColor(255);
		//_fbo_tmp.draw(0,0);
		////_fbo2.getTextureReference().draw(0,0);

		//ofPopStyle();

		
	}

};



#endif

