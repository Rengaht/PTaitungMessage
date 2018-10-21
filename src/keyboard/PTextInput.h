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
	wstring _wstr;
	//string _str;
	int _cursor;
	wstring _wstr_spelling;
	int _cursor_spelling;


	int _max_text;

	FrameTimer _timer_blink;
	bool _focus;

public:
	ofxTrueTypeFontUC Font;	
	ofEvent<int> _event_enter;

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
		ofAddListener(kb_->_event_spelling,this,&PTextInput::onSpellingUpdate);		
		ofAddListener(kb_->_event_back,this,&PTextInput::onkeyBack);
		ofAddListener(kb_->_event_enter,this,&PTextInput::onKeyEnter);
		ofAddListener(kb_->_event_left,this,&PTextInput::onKeyLeft);
		ofAddListener(kb_->_event_right,this,&PTextInput::onKeyRight);
		reset();

		_timer_blink.restart();
		
	}
	void setFocus(bool set_){
		_focus=set_;
		_cursor=max((int)_wstr.size(),0);
	}
	void reset(){
		//_str.clear();
		_wstr.clear();
		_wstr_spelling.clear();

		_cursor=_cursor_spelling=0;
		_focus=false;

	}
	void update(float dt_){
		_timer_blink.update(dt_);
	}
	void draw(){
		ofPushMatrix();
		ofTranslate(_pos);
		//ofTranslate(0,_font_size);
		
		string pre_=Param::ws2utf8(_wstr.substr(0,_cursor));
		string last_=Param::ws2utf8(_wstr.substr(_cursor,_wstr.length()-_cursor));
			if(_wstr.length()>0){
				Font.drawString(pre_,0,0);
			}

			if(_focus){
				float x=Font.getStringBoundingBox(pre_,0,0).width;
				ofTranslate(x+_font_size*CURSOR_SPACE,0);
				ofPushStyle();
				ofSetColor(255,255*_timer_blink.valFade());
					ofDrawLine(0,_font_size*.2,0,-_font_size*1.2);
				ofPopStyle();
			
				ofTranslate(_font_size*CURSOR_SPACE,0);
			}
			if(_wstr.length()>_cursor) Font.drawString(last_,0,0);

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

		wstring ws=Param::utf82ws(set_);

		if(_wstr.size()>=_max_text) return;

		_wstr.insert(_cursor,ws);
		_cursor++;
		_cursor_spelling=_cursor;

		if(_wstr.size()==0) _rect=ofRectangle(_pos.x,_pos.y,_font_size*_max_text/3,_font_size);
		else _rect=Font.getStringBoundingBox(Param::ws2utf8(_wstr),_pos.x,_pos.y);
	}
	void onSpellingUpdate(string& set_){

		if(!_focus) return;

		wstring ws=Param::utf82ws(set_);

		_wstr.erase(_cursor_spelling,_wstr_spelling.size());
		_cursor=_cursor_spelling;

		_wstr_spelling=ws;

		if(ws.size()>0){
			_wstr.insert(_cursor,ws);
			_cursor_spelling=_cursor;
			_cursor+=ws.size();
		}else{
			_cursor_spelling=_cursor;
		}

		if(_wstr.size()==0) _rect=ofRectangle(_pos.x,_pos.y,_font_size*_max_text/3,_font_size);
		else _rect=Font.getStringBoundingBox(Param::ws2utf8(_wstr),_pos.x,_pos.y);
	}
	
	void onKeyEnter(int& e){
		if(!_focus) return;
		int t=0;
		ofNotifyEvent(_event_enter,t);
	}
	void onKeyLeft(int& e){
		if(!_focus) return;
		_cursor=ofClamp(_cursor-1,0,_wstr.length());
		
		if(_wstr_spelling.size()==0) _cursor_spelling=_cursor;
	}
	void onKeyRight(int& e){
		if(!_focus) return;
		_cursor=ofClamp(_cursor+1,0,_wstr.length());

		if(_wstr_spelling.size()==0) _cursor_spelling=_cursor;
	}
	void onkeyBack(int &e){
		if(!_focus) return;
		if(_cursor>0){
			_wstr.erase(_cursor-1,1);
			_cursor--;
			if(_wstr_spelling.size()==0) _cursor_spelling=_cursor;
			else{
				//TODO!!!			
			}
		}
	}
	bool mouseInside(float mousex,float mousey){
		return _rect.inside(mousex,mousey);				
	}

	string getValue(){
		return Param::ws2utf8(_wstr);
	}


};
#endif