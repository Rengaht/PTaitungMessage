#pragma once

#ifndef TEXT_RUNNER_H
#define TEXT_RUNNER_H

#include <codecvt>
#include "FrameTimer.h"
#include "ofxTrueTypeFontUC.h"
#include "Parameter.h"

class TextRunner{
	
private:
	wstring _str;
	FrameTimer _timer_in;
	FrameTimer _timer_out;

	int _index_text;
	ofVec2f _pos;
	bool _cont;
public:
	static ofxTrueTypeFontUC TextFont;
	static float CharWid;
	static int TimePerText;

	TextRunner(){}
	TextRunner(float x,float y,wstring set_){
		_str=set_;		
		_timer_in=FrameTimer(TimePerText*_str.size());
		_pos=ofVec2f(x,y);

		_cont=false;
	}
	TextRunner(float y,wstring set_){
		_str=set_;		
		_timer_in=FrameTimer(TimePerText*_str.size());

		float x=(1920-_str.size()*CharWid)/2.0;
		_pos=ofVec2f(x,y);

		_cont=false;
	}
	void setDelay(int set_){
		_timer_in=FrameTimer(TimePerText*_str.size(),set_);
	}
	void setCont(bool set_){
		_cont=set_;
		if(_cont){
			_timer_out=FrameTimer(_str.size()*TimePerText*.75,_str.size()*TimePerText);			
		}
	}
	void reset(){
		_index_text=0;
		_timer_in.reset();
		_timer_out.reset();
	}
	void start(){
		_timer_in.restart();
		_timer_out.restart();
	}
	void restart(){
		reset();
		start();
	}
	void update(float dt_){
		
		_timer_in.update(dt_);
		_timer_out.update(dt_);
		if(_cont && _timer_out.finish()) restart();

		_index_text=ofClamp(floor(_timer_in.val()*_str.size()),0,_str.size()-1);
	}
	void draw(){	
		float alpha=_cont?1.0-_timer_out.valEaseInOut():1.0;
		draw(alpha);
	}
	void draw(float alpha){
		if(_index_text<0 || _index_text>=_str.size()) return;

		ofPushMatrix();
		ofTranslate(_pos);

	
		ofPushStyle();
		ofSetColor(255,255*alpha);
		float x=0;
		
		for(int i=0;i<_index_text;++i){
			TextFont.drawString(Param::ws2utf8(_str.substr(i,1)),x,0);		
			x+=CharWid;
		}
		ofSetColor(255,255*(_timer_in.val()*_str.size()-_index_text)*alpha);
		TextFont.drawString(Param::ws2utf8(_str.substr(_index_text,1)),x,0);				

		ofPopStyle();

		ofPopMatrix();
	}


};



#endif