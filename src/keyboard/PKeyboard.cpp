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
			string k=_param.getAttribute("KEY","sign",s,j);

			if(s=="NULL") x+=w*wid;
			else{
				
				if(k=="SPACE") s=" ";

				PKey kk=PKey(s,k,x+wid*KEY_MARGIN,i*wid+wid*KEY_MARGIN,w*wid-wid*KEY_MARGIN*2,h*wid-2*wid*KEY_MARGIN);
				


				if(s=="ENTER") kk.setImage("icon/enter.png");
				else if(s=="LEFT") kk.setImage("icon/arrow-left.png");
				else if(s=="RIGHT") kk.setImage("icon/arrow-right.png");
				else if(s=="BCK") kk.setImage("icon/backspace.png");
				else if(s=="LAN") kk.setImage("icon/language.png");
				else if(s=="SHIFT") kk.setImage("icon/shift.png");
				else if(k=="SPACE") kk.setImage("icon/space.png");

				key_.push_back(kk);
				
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

	_rect_select=ofRectangle(pos_.x+size_.x+_size_key.x*.5,pos_.y,wid*3,size_.y);

	PKey::Font.loadFont("font/NotoSansCJKtc-Regular.otf",font_size_);

	loadKeyLayout("layout/keylayout_en.xml",_key_en);
	loadKeyLayout("layout/keylayout_en_cap.xml",_key_en_cap);
	loadKeyLayout("layout/keylayout_number.xml",_key_number);
	loadKeyLayout("layout/keylayout_ch.xml",_key_chinese);

	setLanguage(EN);
	reset();
}
void PKeyboard::reset(){
	resetSpelling();
	_wstr.clear();
	_show_select=false;
	_cursor=0;
	_cursor_spelling=0;

	//setLanguage(EN);
}
void PKeyboard::resetSpelling(){

	
	if(_wstr_spelling.size()>0){
		_wstr.erase(_cursor_spelling,_wstr_spelling.size());
		_cursor=_cursor_spelling;
	}
	ofNotifyEvent(_event_input,_wstr);
	ofNotifyEvent(_event_cursor,_cursor);

	_wstr_spelling.clear();
	_str_spelling.clear();

}

void PKeyboard::draw(bool debug_){

	ofPushMatrix();
	ofTranslate(_pos);

	float x=0;
	float y=0;

	if(_language==CHINESE){
		//PKey::Font.drawString(_utf8_spelling,0,0);	

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
			//ofLog()<<k._key;
			if(k._key=="SPACE"){
				
				if(_language==CHINESE) keyEventP(k);
				else keyEvent(k);

			}else if(k._key=="BCK"){
				onKeyBack();
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
				onKeyEnter();
			}else if(k._key=="LEFT"){
				onKeyLeft();
				
			}else if(k._key=="RIGHT"){
				onKeyRight();
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
	
	if(_language==CHINESE){
		_str_spelling.clear();
		//_utf8_spelling.clear();
		//ofNotifyEvent(_event_spelling,_str_spelling);

		updateSpelling();

	}
	string s=key_._word;

	
	if(_wstr.size()<_max_text_length-1){
		_wstr.insert(_cursor,Param::utf82ws(s));
		_cursor++;	
	}

	_cursor_spelling=_cursor;

	ofNotifyEvent(_event_input,_wstr);
	ofNotifyEvent(_event_cursor,_cursor);
	_show_select=false;

	
}
void PKeyboard::keyEventP(PKey& key_){
		
		if(key_._key!="SPACE"){
			_str_spelling.push_back(key_._word);
			updateSpelling();
		}

		if(_str_spelling.size()>3 || key_._key=="5"||key_._key=="2"||key_._key=="3"||key_._key=="4"||key_._key=="SPACE"){
			
			auto ww=_db.key2CWord(getSpellingStr());
			//ofNotifyEvent(_event_input,ww);
			_select.clear();

			if(ww.size()==0){
				resetSpelling();
			}else{
				int m=0;
				float wid=_size_key.x;
				for(auto& w:ww){
					_select.push_back(PKey(w,w,_size.x+wid*.5+wid*(m%3)+wid*KEY_MARGIN,wid*floor(m%9/3)+wid*KEY_MARGIN,
											wid-wid*2*KEY_MARGIN,wid-wid*2*KEY_MARGIN));
					m++;
				}
				_index_select=0;
				_show_select=true;
			}
			
		}
	
}
string PKeyboard::getSpellingStr(){
	string str_="";
	for(auto& s:_str_spelling) str_+=s;
	return str_;
}
void PKeyboard::updateSpelling(){
	
	string set_=getSpellingStr();
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

	ofNotifyEvent(_event_input,_wstr);
	ofNotifyEvent(_event_cursor,_cursor);

}

void PKeyboard::onKeyLeft(){
	if(_show_select){
		if(_index_select>=MSELECT_SHOW) _index_select-=MSELECT_SHOW;
	}else{
		_cursor=ofClamp(_cursor-1,0,_wstr.length());
		if(_wstr_spelling.size()==0) _cursor_spelling=_cursor;
		ofNotifyEvent(_event_cursor,_cursor);

	}
}
void PKeyboard::onKeyRight(){
	if(_show_select){
		if(_index_select+MSELECT_SHOW<_select.size()) _index_select+=MSELECT_SHOW;
	}else{
		_cursor=ofClamp(_cursor+1,0,_wstr.length());
		if(_wstr_spelling.size()==0) _cursor_spelling=_cursor;
		ofNotifyEvent(_event_cursor,_cursor);

	}
}
void PKeyboard::onKeyEnter(){
	int t=0;
	resetSpelling();

	ofNotifyEvent(_event_enter,t);
}
void PKeyboard::onKeyBack(){
	if(_cursor>0){
		if(_wstr_spelling.size()==0){
			_wstr.erase(_cursor-1,1);
			_cursor--;
			_cursor_spelling=_cursor;

			ofNotifyEvent(_event_input,_wstr);
			ofNotifyEvent(_event_cursor,_cursor);
		}else{
			auto it_=_str_spelling.begin();
			advance(it_,_cursor-_cursor_spelling-1);
			_str_spelling.erase(it_);
			_cursor--;
			updateSpelling();
		}
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
	
	resetSpelling();
	//_utf8_spelling.clear();
}

bool PKeyboard::mouseInside(float mousex,float mousey){

	if(_language==CHINESE)
		return  _rect.inside(mousex,mousey) || _rect_select.inside(mousex,mousey);	

	return _rect.inside(mousex,mousey);
}

void PKeyboard::updateInput(wstring str_,int pcursor_,int maxtext_){
	_max_text_length=maxtext_;
	_wstr=str_;
	_cursor_spelling=_cursor=pcursor_;


	ofNotifyEvent(_event_input,_wstr);
	ofNotifyEvent(_event_cursor,_cursor);
}