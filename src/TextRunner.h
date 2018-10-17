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
	bool _fadeout;

public:
	static ofxTrueTypeFontUC TextFont;
	static float CharWid;
	static int TimePerText;

	TextRunner(){}
	TextRunner(float x,float y,wstring set_){
		_str=set_;		
		_timer_in=FrameTimer(TimePerText*_str.size());
		_timer_out=FrameTimer(_str.size()*TimePerText*.75,_str.size()*TimePerText);	
		_pos=ofVec2f(x,y);

		_cont=false;
		_fadeout=true;
	}
	TextRunner(float y,wstring set_){
		_str=set_;		
		_timer_in=FrameTimer(TimePerText*_str.size());
		_timer_out=FrameTimer(_str.size()*TimePerText*.75,_str.size()*TimePerText);	

		float x=(1920-_str.size()*CharWid)/2.0;
		_pos=ofVec2f(x,y);

		_cont=false;
		_fadeout=true;
	}
	/*void setDelay(int set_){
		_timer_in=FrameTimer(TimePerText*_str.size(),set_);
	}*/
	void setFadeOut(bool set_){
		_fadeout=set_;
	}
	void setCont(bool set_){
		_cont=set_;		
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
	bool finish(){
		if(_fadeout) return _timer_out.finish();
		else return _timer_in.finish();
	}
	float getTotalTime(){
		return _timer_in.getDue();
	}

	void update(float dt_){
		
		_timer_in.update(dt_);
		_timer_out.update(dt_);
		//ofLog()<<_timer_in.val()<<"  "<<_timer_out.val();

		if(_cont && _timer_out.finish()) restart();

		_index_text=ofClamp(floor(_timer_in.val()*_str.size()),0,_str.size()-1);
	}
	void draw(float alpha=1.0){
		if(_index_text<0 || _index_text>=_str.size()) return;


		alpha*=(_fadeout)?1.0-_timer_out.valEaseInOut():1.0;

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