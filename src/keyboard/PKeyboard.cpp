#include "PKeyboard.h"
#include "Parameter.h"

ofxTrueTypeFontUC PKey::Font;
string PDatabase::db_path="gcin.db";
string PDatabase::cin_path="CnsMPSII107-03_GCIN.cin";		
string PDatabase::map_path="cns.map";


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
	float wid=_size_key.x;

	for(int i=0;i<mline;++i){

		float x=_param.getAttribute("LINE","dx",0.0,i)*wid;
		_param.pushTag("LINE",i);
		int mkey=_param.getNumTags("KEY");

		for(int j=0;j<mkey;++j){
			string s=_param.getValue("KEY","",j);
			/*wstring ws=Param::utf82ws(s);
			string u8s=Param::ws2utf8(ws);*/

			float w=_param.getAttribute("KEY","wid",1.0,j);
			float h=_param.getAttribute("KEY","hei",1.0,j);
			string k=_param.getAttribute("KEY","sign","",j);

			if(s=="NULL") x+=w*wid;
			else{
				if(k=="") key_.push_back(PKey(s,s,x+wid*KEY_MARGIN,i*wid+wid*KEY_MARGIN,w*wid-wid*KEY_MARGIN*2,h*wid-2*wid*KEY_MARGIN));
				else  key_.push_back(PKey(s,k,x+wid*KEY_MARGIN,i*wid+wid*KEY_MARGIN,w*wid-wid*KEY_MARGIN*2,h*wid-2*wid*KEY_MARGIN));
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

	float wid=_size.x/KEY_PER_LINE;
	_size_key=ofVec2f(wid,wid);

	_rect_select=ofRectangle(pos_.x+size_.x,pos_.y,wid*3,size_.y);

	PKey::Font.loadFont("font/NotoSansCJKtc-Regular.otf",font_size_);

	loadKeyLayout("layout/keylayout_en.xml",_key_en);
	loadKeyLayout("layout/keylayout_en_cap.xml",_key_en_cap);
	loadKeyLayout("layout/keylayout_number.xml",_key_number);
	loadKeyLayout("layout/keylayout_ch.xml",_key_chinese);

	setLanguage(EN);
}
void PKeyboard::draw(bool debug_){

	ofPushMatrix();
	ofTranslate(_pos);

	float x=0;
	float y=0;

	if(_language==CHINESE){
		PKey::Font.drawString(_utf8_complex,0,0);	

		if(_select.size()>0){
			auto it_=_select.begin();
			advance(it_,_index_select);
			for(int i=0;i<MSELECT_SHOW&&it_!=_select.end();++i,++it_){
				(*it_).draw();				
			}
		}
	}
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
				
				if(_language==CHINESE) keyEventP(k);
				else keyEvent(k);

			}else if(k._key=="BCK"){
				ofNotifyEvent(_event_back, t);
			}else if(k._key=="&123"){
				setLanguage(NUMBER);
			}else if(k._key=="abc"){
				setLanguage(EN);
			}else if(k._key=="LAN"){
				
				if(_language==EN || _language==ENCAP) setLanguage(CHINESE);
				else setLanguage(EN);

			}else if(k._key=="SHIFT"){
				if(_language==EN) setLanguage(ENCAP);
				else if(_language==ENCAP) setLanguage(EN);
			}else if(k._key=="ENTER"){
				ofNotifyEvent(_event_enter,t);
			}else if(k._key=="LEFT"){
				
				if(_language==CHINESE){
					if(_index_select>MSELECT_SHOW) _index_select-=MSELECT_SHOW;
				}else ofNotifyEvent(_event_left,t);

			}else if(k._key=="RIGHT"){
				if(_language==CHINESE){
					if(_index_select+MSELECT_SHOW<_select.size()) _index_select+=MSELECT_SHOW;
				}else ofNotifyEvent(_event_right,t);
			}else{
				if(_language==CHINESE) keyEventP(k);
				else keyEvent(k);
			}
		}
	}

	if(_select.size()>0){
		auto it_=_select.begin();
		advance(it_,_index_select);
		for(int i=0;i<MSELECT_SHOW &&it_!=_select.end();++i,++it_){
			if((*it_).inside(pt_)){
				keyEvent(*it_);			
				_select.clear();
				break;
			}
		}
	}
	return true;
}
void PKeyboard::keyEvent(PKey& key_){
	string s=key_._word;
	ofNotifyEvent(_event_input,s);
}
void PKeyboard::keyEventP(PKey& key_){
		
		if(key_._key!="SPACE"){
			_str_complex+=key_._word;
			_utf8_complex+=key_._u8s;
		}

		if(key_._key=="5"||key_._key=="2"||key_._key=="3"||key_._key=="4"||key_._key=="SPACE"){
			
			auto ww=_db.key2CWord(_str_complex);
			//ofNotifyEvent(_event_input,ww);
			_select.clear();
			int m=0;
			float wid=_size_key.x;
			for(auto& w:ww){
				_select.push_back(PKey(w,w,_size.x+wid*(m%3)+wid*KEY_MARGIN,wid*floor(m%9/3)+wid*KEY_MARGIN,
										wid-wid*2*KEY_MARGIN,wid-wid*2*KEY_MARGIN));
				m++;
			}
			_index_select=0;

			
			_str_complex.clear();
			_utf8_complex.clear();
		}
	
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
		case CHINESE:
			_key=&_key_chinese;
			break;
	}
	_language=set_;
	
	_str_complex.clear();
	_utf8_complex.clear();
}

bool PKeyboard::mouseInside(float mousex,float mousey){

	if(_language==CHINESE)
		return  _rect.inside(mousex,mousey) || _rect_select.inside(mousex,mousey);	

	return _rect.inside(mousex,mousey);
}