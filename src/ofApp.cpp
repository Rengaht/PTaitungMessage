#include "ofApp.h"
#include "SceneHome.h"
#include "SceneIntro.h"
#include "SceneColor.h"
#include "SceneName.h"
#include "SceneRecord.h"
#include "SceneInfo.h"



//--------------------------------------------------------------
void ofApp::setup(){

	ofSetVerticalSync(true);
	ofEnableSmoothing();
	//ofSoundStreamSetup(0,NUM_CHANNELS,this, SAMPLE_RATE,BUFFER_SIZE,4);

	_sound_stream.printDeviceList();
	//_sound_stream.setDevice(_sound_stream.getDeviceList()[1]);
	_sound_stream.setup(this, 0, NUM_CHANNELS, SAMPLE_RATE, BUFFER_SIZE, 4);
	
	_fft = ofxFft::create(BUFFER_SIZE, OF_FFT_WINDOW_BARTLETT);
	audioInput = new float[BUFFER_SIZE];
	fftOutput = new float[_fft->getBinSize()];
	eqFunction = new float[_fft->getBinSize()];
	eqOutput = new float[_fft->getBinSize()];	
	for(int i = 0; i < _fft->getBinSize(); i++)
		eqFunction[i] = (float) (_fft->getBinSize() - i) / (float) _fft->getBinSize();

	_param=new Param();

	_serial.listDevices();
	_serial.setup(0,19200);

	_now_millis=ofGetElapsedTimeMillis();

	SceneBase::WinScale=max(ofGetWidth()/1920.0,ofGetHeight()/1080.0);
	SceneBase::SoundButton.load("sound/icon.wav");
	TextRunner::TextFont.loadFont("font/NotoSansCJKtc-Regular.otf",21);
	TextRunner::CharWid=TextRunner::TextFont.getFontSize()*1.4;

	ofDisableArbTex();
	_img_back.loadImage("ui/back.png");
	_img_back.getTextureReference().setTextureWrap(GL_MIRRORED_REPEAT,GL_MIRRORED_REPEAT);

	SceneBase::ImgPillLeft=new ofImage[4];
	SceneBase::ImgPillRight=new ofImage[4];
	SceneBase::ImgPillLeft[0]=ofImage("ui/ground-1.png");
	SceneBase::ImgPillLeft[1]=ofImage("ui/sky-1.png");
	SceneBase::ImgPillLeft[2]=ofImage("ui/white-1.png");
	SceneBase::ImgPillLeft[3]=ofImage("ui/yellow-1.png");

	SceneBase::ImgPillRight[0]=ofImage("ui/ground-2.png");
	SceneBase::ImgPillRight[1]=ofImage("ui/sky-2.png");
	SceneBase::ImgPillRight[2]=ofImage("ui/white-2.png");
	SceneBase::ImgPillRight[3]=ofImage("ui/yellow-2.png");

	
	_show_keyboard=false;
	_keyboard=new PKeyboard(ofVec2f(450,720),ofVec2f(1020,340),18);


	loadScene();
	/*_mode=PEMPTY;
	setScene(PStatus::PHOME);*/

	_mode_pre=PEMPTY;
	_mode=PRECORD;
	_scene[_mode]->init();

	_mesh_back.addVertex(ofVec2f(0,0));
	_mesh_back.addVertex(ofVec2f(ofGetWidth(),0));
	_mesh_back.addVertex(ofVec2f(ofGetWidth(),ofGetHeight()));

	_mesh_back.addVertex(ofVec2f(ofGetWidth(),ofGetHeight()));
	_mesh_back.addVertex(ofVec2f(0,ofGetHeight()));
	_mesh_back.addVertex(ofVec2f(0,0));
	
	_mesh_back.addTexCoord(ofVec2f(0,0));
	_mesh_back.addTexCoord(ofVec2f(1,0));
	_mesh_back.addTexCoord(ofVec2f(1,1));
	
	_mesh_back.addTexCoord(ofVec2f(1,1));
	_mesh_back.addTexCoord(ofVec2f(0,1));
	_mesh_back.addTexCoord(ofVec2f(0,0));

	ofAddListener(SceneBase::sceneInFinish,this,&ofApp::onSceneInFinish);
	ofAddListener(SceneBase::sceneOutFinish,this,&ofApp::onSceneOutFinish);

	

}

//--------------------------------------------------------------
void ofApp::update(){

	float dt_=ofGetElapsedTimeMillis()-_now_millis;
	_now_millis+=dt_;

	if(_in_transition && _mode_pre!=PStatus::PEMPTY)_scene[_mode_pre]->update(dt_);
	
	_scene[_mode]->update(dt_);

	float tp=_now_millis/15000.0;
	_mesh_back.setTexCoord(0,ofVec2f(tp*1,0));
	_mesh_back.setTexCoord(1,ofVec2f((1+tp)*1,0));
	_mesh_back.setTexCoord(2,ofVec2f((1+tp)*1,1));
	_mesh_back.setTexCoord(3,ofVec2f((1+tp)*1,1));
	_mesh_back.setTexCoord(4,ofVec2f(tp*1,1));
	_mesh_back.setTexCoord(5,ofVec2f(tp*1,0));

	

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetBackgroundColor(0);

	ofEnableBlendMode(OF_BLENDMODE_ADD);

	drawBackground();

	if(_in_transition && _mode_pre!=PStatus::PEMPTY){
#ifdef DRAW_DEBUG
		_scene[_mode_pre]->drawScaled(true);
#else
		_scene[_mode_pre]->drawScaled(false);
#endif
	}
	


#ifdef DRAW_DEBUG
	
	_scene[_mode]->drawScaled(true);

	ofDrawBitmapString("fps= "+ofToString(ofGetFrameRate()),10,10);
	ofDrawBitmapString("sleep= "+ofToString((1-SceneBase::_timer_sleep.val())*Param::val()->_time_sleep/1000.0),10,20);
	if(_show_keyboard){
		ofPushMatrix();
		ofScale(SceneBase::WinScale,SceneBase::WinScale);
			_keyboard->draw(true);
		ofPopMatrix();
	}
#else
	_scene[_mode]->drawScaled(false);
	if(_show_keyboard){
		ofPushMatrix();
		ofScale(SceneBase::WinScale,SceneBase::WinScale);
			_keyboard->draw();
		ofPopMatrix();
	}	
#endif
	

	
}
void ofApp::drawBackground(){
	ofEnableNormalizedTexCoords();
	_img_back.bind();
	_mesh_back.draw();
	_img_back.unbind();

	ofDisableNormalizedTexCoords();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch(key){
		case 'a':
			setScene(PHOME);
			break;
		case 'f':
			ofToggleFullscreen();
			break;
		case 'g':
			PDatabase::generate();
			break;		
		case 'k':
			wstring tmp_=Param::val()->getRandomQuestion();
			wstring tmp2=L"§A¦n";
			ofFile file_;
			file_.open(Param::val()->_csv_record,ofFile::Append);
			file_<<","
				<<_user_id<<","
				<<ofToString(_select_color+1)<<","
				<<Param::ws2utf8Output(tmp_)<<","
				<<Param::ws2utf8Output(tmp2)<<endl;

			file_.close();
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bool trigger_=_scene[_mode]->handleMousePressed(x,y);
	if(_show_keyboard){
		bool ktrigger_=_keyboard->checkMouse(ofPoint(x/SceneBase::WinScale,y/SceneBase::WinScale));
		if(!trigger_ && !ktrigger_){
			setShowKeyboard(false);
		}else{
			SceneBase::_timer_sleep.restart();
		}
	}
}


void ofApp::onSceneInFinish(int &e){	
	ofLog()<<"scene "<<e<<" in finish!";	
	_in_transition=false;
}
void ofApp::onSceneOutFinish(int &e){
	ofLog()<<"scene "<<e<<" out finish!";
	//if(e==_mode_pre) _scene[_mode_pre]->reset();
	
}


void ofApp::loadScene(){
	_scene[0]=new SceneHome(this);
	_scene[1]=new SceneIntro(this);
	_scene[2]=new SceneColor(this);
	_scene[3]=new SceneName(this);
	_scene[4]=new SceneRecord(this);
	_scene[5]=new SceneInfo(this);

}

void ofApp::setScene(PStatus set_){

	ofLog()<<"set scene: "<<set_;
	
	_mode_pre=_mode;
	
	_mode=set_;

	if(_mode_pre!=PEMPTY) _scene[_mode_pre]->end();
	if(_mode_pre==PINFO){
		saveUserData();
	}
	
	_scene[_mode]->init();
	
	setShowKeyboard(false);
	_keyboard->reset();
	
	_in_transition=true;

	
}

void ofApp::setSelectColor(int set_){
	_select_color=set_;
}
int ofApp::getSelectColor(){
	return _select_color;
}

void ofApp::setRecording(bool set_){
	
	string tmp_file=ofToDataPath("tmp.wav");
	

	if(set_){
		ofLog()<<"Start recording\n";

		_user_id=ofGetTimestampString("%Y%m%d%H%M%S");
		_path_record=_param->_folder_export+_user_id+".wav";

		ofLog()<<"----"<<_path_record<<"----";

	/*	ofFile tmp_(tmp_file);
		tmp_.remove();*/
		
		_recorder.setup(_path_record,SAMPLE_RATE,NUM_CHANNELS);
		_recorder.setFormat(SF_FORMAT_WAV | SF_FORMAT_PCM_16);
		
		_recording=set_;
	}else{
		_recording=set_;

		ofLog()<<"Stop recording";		
		_recorder.finalize();
		
		/*ofFile copy_(tmp_file);
		copy_.copyTo(_path_record,true,true);
	
		copy_.remove();*/

	}
	
}
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
	if(_mode!=PRECORD) return;

	if(_recording)
		_recorder.addSamples(input,bufferSize);
		//_wav_recorder.addSample(input,nChannels,bufferSize);

	memcpy(audioInput, input, sizeof(float) * bufferSize);


	try{
		//BmFFT::getSpectrum(BUFFER_SIZE, input, _tmp_fft);
		_fft->setSignal(audioInput);
		memcpy(fftOutput, _fft->getAmplitude(), sizeof(float) * _fft->getBinSize());

		for(int i = 0; i < _fft->getBinSize(); i++)
			eqOutput[i] = fftOutput[i] * eqFunction[i];

		//_fft->setPolar(eqOutput, _fft->getPhase());	

		

	}catch(exception& e){
		ofLog()<<e.what();
	}
	//calcVolume(input,bufferSize,nChannels);

}
void ofApp::calcVolume(float* data,int bufferSize,int nChannels){
	
	float cur_vol=0;
	for(int i=0;i<bufferSize;++i){
		if(nChannels==2){
			float left_=ofClamp(data[i*2],-1,1)*0.5;
			float right_=ofClamp(data[i*2+1],-1,1)*0.5;
			cur_vol+=left_*left_*Param::val()->_spectrum_scale;
			cur_vol+=right_*right_*Param::val()->_spectrum_scale;
		}else{
			cur_vol+=data[i]*Param::val()->_spectrum_scale;
		}
	}

	cur_vol/=(float)(bufferSize*nChannels);	
	//cur_vol=sqrt(cur_vol);
	ofLog()<<cur_vol;


	//_volume_now=cur_vol;
	
	_volume_now*=0.8;
	_volume_now+=0.2*cur_vol;
}




void ofApp::playRecord(){

	if(_player_record.isLoaded()) _player_record.unloadSound();
	_player_record.loadSound(_path_record);

	_player_record.play();
	

}

void ofApp::setFFT(bool set_){
	_use_fft=set_;
}


void ofApp::setUserName(wstring set_){
	_user_name=set_;
}
void ofApp::setUserEmail(wstring set_){
	_user_email=set_;
}
void ofApp::setUserPhone(wstring set_){
	_user_phone=set_;
}
void ofApp::saveUserData(){
	
	if(_user_id==""){
		ofLog()<<"Save failed! No user id!";
		return;
	}

	
	string csv_row=","+_user_id+","+ofToString(_select_color+1)+","
				   +Param::ws2utf8Output(_user_name)+","
				   +Param::ws2utf8Output(_user_email)+","
				   +Param::ws2utf8Output(_user_phone)+"\n";
	ofLog()<<"save data:"<<csv_row<<endl;


	/*_csv_user_output.load(Param::val()->_csv_record);
	ofLog()<<"load csv data with "<<_csv_user_output.getNumRows()<<" rows!";*/

	/*_csv_user_output.clear();
	_csv_user_output.addRow(row_);
	_csv_user_output.save(Param::val()->_csv_record);*/

	ofFile file_;
	file_.open(Param::val()->_csv_record,ofFile::Append);
	file_<<","
		 <<_user_id<<","
		 <<ofToString(_select_color+1)<<","
		 <<Param::ws2utf8Output(_user_name)<<","
		 <<Param::ws2utf8Output(_user_email)<<","
		 <<Param::ws2utf8Output(_user_phone)<<endl;

	file_.close();

	sendUpdateOsc();
}

void ofApp::setShowKeyboard(bool set_,PKeyboard::PLANGUAGE lan_){
	_show_keyboard=set_;
	_keyboard->setLanguage(lan_);	
	//_keyboard->reset();
}

PKeyboard* ofApp::getKeyboard(){
	return _keyboard;
}
void ofApp::updateKeyboardInput(wstring str_,int cursor_,int max_){
	_keyboard->updateInput(str_,cursor_,max_);
}


void ofApp::sendUpdateOsc(){
	ofxOscSender sender;
	sender.setup(Param::val()->_osc_address,Param::val()->_osc_port);

	ofxOscMessage message;
	message.setAddress("/update");
	message.addIntArg(1);
	sender.sendMessage(message);
	ofLog()<<"send osc!";	
}
void ofApp::exit(){
	
}

void ofApp::sendVolumeLight(){
	if(!_serial.isInitialized()) return;
	
	int vol_=ofClamp(eqOutput[0]*2*255,0,255);
	//ofLog()<<vol_;

	unsigned char c(vol_);
	
	//ofLog()<<(int)c;

	_serial.writeByte(c);	
	_serial.flush();
}
