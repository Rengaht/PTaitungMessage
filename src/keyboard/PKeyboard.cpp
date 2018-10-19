#include "PKeyboard.h"

ofxTrueTypeFontUC PKey::Font;
string PDatabase::db_path="gcin.db";
string PDatabase::cin_path="CnsMPSII107-03_GCIN.cin";		
string PDatabase::table_name="p2c";

void PKeyboard::loadKeyLayout(string layout_file,list<PKey>& key_){
	

	//string layout_file="keylayout_en.xml";
	ofxXmlSettings _param;

	if(_param.loadFile(layout_file)){
		ofLog()<<layout_file<<" loaded!";
	}else{
		ofLog()<<"Unable to load xml check data/ folder";
		return;
	}

	int mline=_param.getNumTags("LINE");		

	float wid=min(_size.x/KEY_PER_LINE,_size.y/mline);
	_size_key=ofVec2f(wid,wid);

	for(int i=0;i<mline;++i){

		float x=_param.getAttribute("LINE","dx",0.0,i)*wid;
		_param.pushTag("LINE",i);
		int mkey=_param.getNumTags("KEY");

		for(int j=0;j<mkey;++j){
			string s=_param.getValue("KEY","",j);
			float w=_param.getAttribute("KEY","wid",1.0,j);

			if(s=="NULL") x+=w*wid;
			else{
				key_.push_back(PKey(s,x+wid*KEY_MARGIN,i*wid+wid*KEY_MARGIN,w*wid-wid*KEY_MARGIN*2,wid-2*wid*KEY_MARGIN));
				//ofLog()<<s;
				x+=w*wid;
			}
		}
		
		_param.popTag();
	}

	
}
PKeyboard::PKeyboard(ofVec2f pos_,ofVec2f size_,int font_size_){

	_pos=pos_;
	_size=size_;
	_rect=ofRectangle(pos_,size_.x,size_.y);
	PKey::Font.loadFont("font/NotoSansCJKtc-Regular.otf",font_size_);

	loadKeyLayout("layout/keylayout_en.xml",_key_en);
	loadKeyLayout("layout/keylayout_en_cap.xml",_key_en_cap);
	loadKeyLayout("layout/keylayout_number.xml",_key_number);

	setLanguage(EN);
}
void PKeyboard::draw(bool debug_){

	ofPushMatrix();
	ofTranslate(_pos);

	float x=0;
	float y=0;

	for(auto& k:*_key) k.draw();

	ofPopMatrix();

	if(debug_) drawDebug();
}
void PKeyboard::drawDebug(){
	ofPushStyle();
	ofSetColor(0,255,0);
	ofNoFill();
	ofDrawRectangle(_rect);
	ofPopStyle();
}
bool PKeyboard::checkMouse(ofPoint pt_){
	
	
	if(!mouseInside(pt_.x,pt_.y)) return false;
	pt_-=_pos;
	int t=0;
	for(auto& k:*_key){
		if(k.inside(pt_)){
			ofLog()<<k._key;
			if(k._key=="SPACE"){
				string s=" ";
				ofNotifyEvent(_event_input,s);
			}else if(k._key=="BCK"){
				ofNotifyEvent(_event_back, t);
			}else if(k._key=="&123"){
				setLanguage(NUMBER);
			}else if(k._key=="abc"){
				setLanguage(EN);
			}else if(k._key=="EN" || k._key=="en"){
			
			}else if(k._key=="SHIFT"){
				if(_language==EN) setLanguage(ENCAP);
				else if(_language==ENCAP) setLanguage(EN);
			}else if(k._key=="ENTER"){
				ofNotifyEvent(_event_enter,t);
			}else if(k._key=="LEFT"){
				ofNotifyEvent(_event_left,t);
			}else if(k._key=="RIGHT"){
				ofNotifyEvent(_event_right,t);
			}else{
				//str_+=k._word;
				ofNotifyEvent(_event_input,k._word);
			}
		}
	}
	return true;
}

void PKeyboard::setLanguage(PLANGUAGE set_){
	switch(set_){
		case EN:
			_key=&_key_en;
			break;
		case ENCAP:
			_key=&_key_en_cap;
			break;
		case NUMBER:
			_key=&_key_number;
			break;
	}
	_language=set_;
}

bool PKeyboard::mouseInside(float mousex,float mousey){
	return _rect.inside(mousex,mousey);
}