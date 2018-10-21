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

	void draw(){
		ofPushMatrix();
		ofTranslate(_rect.position);

		ofPushStyle();
		ofSetColor(255);
		ofNoFill();
		ofDrawRectangle(0,0,_rect.width,_rect.height);			
		ofPopStyle();

		ofPushStyle();
		ofSetColor(255);
			Font.drawString(_u8s,_rect.width/2-_rect_word.width/2,_rect.height/2+_rect_word.height/2);
		ofPopStyle();

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

public:
	enum PLANGUAGE {EN,ENCAP,CHINESE,NUMBER};
	PLANGUAGE _language;
	void setLanguage(PLANGUAGE set_);

	ofEvent<string> _event_input,_event_hint;
	ofEvent<int> _event_enter,_event_back,_event_left,_event_right;

	ofVec2f _pos,_size;		
	ofVec2f _size_key;
	ofRectangle _rect,_rect_select;


	PKeyboard(){}
	PKeyboard(ofVec2f pos_,ofVec2f size_,int font_size_);
	void draw(bool debug_=false);
	void drawDebug();
	bool checkMouse(ofPoint pt_);
	string str_;
	bool mouseInside(float mousex,float mousey);
	void keyEvent(PKey& key_);
	void keyEventP(PKey& key_);
	
	string _str_complex;
	string _utf8_complex;
	

	int _index_select;

};


#endif
