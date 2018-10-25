#pragma once
#ifndef PKEYBOARD_H
#define PKEYBOARD_H


#include "ofxXmlSettings.h"
#include "ofxTrueTypeFontUC.h"

#include "PDatabase.h"
#define KEY_PER_LINE 12
#define KEY_MARGIN 0.05
#define MSELECT_SHOW 9

class PKey{
	ofRectangle _rect;
	ofRectangle _rect_word;
	ofImage _img_icon;
public:
	string _key;
	string _word;
	string _u8s;
	
	static ofxTrueTypeFontUC Font;
	/*PKey(string set_,float x,float y,float wid,float hei){
		_rect=ofRectangle(x,y,wid,hei);
		_word=set_;
		_key=set_;
		_rect_word=Font.getStringBoundingBox(_word,0,0);

	}*/
	PKey(string set_,string key_,float x,float y,float wid,float hei){
		_rect=ofRectangle(x,y,wid,hei);
		_word=set_;
		_key=key_;
		_rect_word=Font.getStringBoundingBox(string(_word.begin(),_word.end()),0,0);


		wstring ws=Param::utf82ws(_word);
		_u8s=Param::ws2utf8(ws);
	}
	void setImage(string img_path){
		_img_icon.loadImage(img_path);
		_rect_word=ofRectangle(0,0,Font.getFontSize()*1.5,Font.getFontSize()*1.5);
	}
	void draw(){
		ofPushMatrix();
		ofTranslate(_rect.position);

		ofPushStyle();
		ofSetColor(255);
		ofNoFill();
		ofDrawRectangle(0,0,_rect.width,_rect.height);			
		ofPopStyle();

		if(_img_icon.bAllocated()){
			_img_icon.draw(_rect.width/2-_rect_word.width/2,_rect.height/2-_rect_word.height/2,
							_rect_word.width,_rect_word.height);
		
		}else{
			ofPushStyle();
			ofSetColor(255);
				Font.drawString(_u8s,_rect.width/2-_rect_word.width/2,_rect.height/2+_rect_word.height/2);
			ofPopStyle();
		}
		ofPopMatrix();
	}
	bool inside(ofPoint pt_){
		return _rect.inside(pt_);
	}
	
};


class PKeyboard{

private:
	void loadKeyLayout(string file_name,list<PKey>& key_);
	list<PKey> _key_en,_key_en_cap,_key_number,_key_chinese;
	PDatabase _db;
	list<PKey>* _key;
	
	list<PKey> _select;
	bool _show_select;

	int _max_text_length;
	bool _show_language;


	//sound
	ofSoundPlayer _sound_button;

public:
	enum PLANGUAGE {EN,ENCAP,CHINESE,NUMBER};
	PLANGUAGE _language;
	void setLanguage(PLANGUAGE set_);

	ofEvent<wstring> _event_input;//_event_spelling;
	ofEvent<int> _event_enter,_event_cursor;//_event_back,_event_left,_event_right;

	ofVec2f _pos,_size;		
	ofVec2f _size_key;
	ofRectangle _rect,_rect_select;


	PKeyboard(){}
	PKeyboard(ofVec2f pos_,ofVec2f size_,int font_size_);
	void draw(bool debug_=false);
	void drawDebug();
	bool checkMouse(ofPoint pt_);
	bool mouseInside(float mousex,float mousey);
	void keyEvent(PKey& key_);
	void keyEventP(PKey& key_);

	void onKeyLeft();
	void onKeyRight();
	void onKeyEnter();
	void onKeyBack();
	
	string getSpellingStr();
	void updateSpelling();

	list<string>_str_spelling;
	wstring _wstr;
	
	wstring _wstr_spelling;
	int _cursor,_cursor_spelling;

	int _index_select;

	void reset();
	void resetSpelling();

	void setShowLanguage(bool set_);
	
	void updateInput(wstring str_,int pcursor_,int maxtext_);
};


#endif
