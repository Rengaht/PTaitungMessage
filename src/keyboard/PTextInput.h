#pragma once
#ifndef PTEXTINPUT_H
#define PTEXT_INPUT_H

#define CURSOR_SPACE .2
#include "PKeyboard.h"
#include "FrameTimer.h"

class PTextInput{
	
	ofVec2f _pos;
	ofRectangle _rect;

	float _font_size;
	string _str;
	int _cursor;

	int _max_text;

	FrameTimer _timer_blink;
	bool _focus;

public:
	ofxTrueTypeFontUC Font;	
	PTextInput(float x,float y,float font_size_,PKeyboard* kb_,
			int max_=10,string font_="font/NotoSansCJKtc-Regular.otf"){
		
		_rect=ofRectangle(x,y-font_size_,font_size_*max_/3,font_size_);
		_pos=ofVec2f(x,y);
		_max_text=max_;

		Font.loadFont(font_,font_size_);
		_font_size=font_size_;

		_timer_blink=FrameTimer(1000);
		_timer_blink.setContinuous(true);

		ofAddListener(kb_->_event_input,this,&PTextInput::onInputUpdate);
		ofAddListener(kb_->_event_back,this,&PTextInput::onkeyBack);
		ofAddListener(kb_->_event_enter,this,&PTextInput::onKeyEnter);
		ofAddListener(kb_->_event_left,this,&PTextInput::onKeyLeft);
		ofAddListener(kb_->_event_right,this,&PTextInput::onKeyRight);
		reset();

		_timer_blink.restart();
		
	}
	void setFocus(bool set_){
		_focus=set_;
		_cursor=max((int)_str.size(),0);
	}
	void reset(){
		_str="";
		_cursor=0;
		_focus=false;
	}
	void update(float dt_){
		_timer_blink.update(dt_);
	}
	void draw(){
		ofPushMatrix();
		ofTranslate(_pos);
		//ofTranslate(0,_font_size);

			if(_str.length()>0){
				Font.drawString(_str.substr(0,_cursor),0,0);
			}

			if(_focus){
				float x=Font.getStringBoundingBox(_str.substr(0,_cursor),0,0).width;
				ofTranslate(x+_font_size*CURSOR_SPACE,0);
				ofPushStyle();
				ofSetColor(255,255*_timer_blink.valFade());
					ofDrawLine(0,_font_size*.2,0,-_font_size*1.2);
				ofPopStyle();
			
				ofTranslate(_font_size*CURSOR_SPACE,0);
			}
			if(_str.length()>_cursor) Font.drawString(_str.substr(_cursor,_str.length()-_cursor),0,0);
		ofPopMatrix();
	}
	void drawDebug(){
		ofPushStyle();
		ofSetColor(0,255,0);
		ofNoFill();

		ofDrawRectangle(_rect);		
		ofPopStyle();
	}
	void onInputUpdate(string& set_){
		
		if(!_focus) return;
		if(_str.size()>=_max_text) return;

		_str.insert(_cursor,set_);
		_cursor++;

		if(_str.size()==0) _rect=ofRectangle(_pos.x,_pos.y,_font_size*_max_text/3,_font_size);
		else _rect=Font.getStringBoundingBox(_str,_pos.x,_pos.y);
	}
	void onKeyEnter(int& e){
		if(!_focus) return;
	}
	void onKeyLeft(int& e){
		if(!_focus) return;
		_cursor=ofClamp(_cursor-1,0,_str.length());
	}
	void onKeyRight(int& e){
		if(!_focus) return;
		_cursor=ofClamp(_cursor+1,0,_str.length());
	}
	void onkeyBack(int &e){
		if(!_focus) return;
		if(_cursor>0){
			_str.erase(_cursor-1,1);
			_cursor--;
		}
	}
	bool mouseInside(float mousex,float mousey){
		return _rect.inside(mousex,mousey);				
	}

	string getValue(){
		return _str;
	}


};
#endif