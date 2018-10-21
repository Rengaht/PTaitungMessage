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
	_fft_band=new float[FFT_NBANDS];
	for(int i=0;i<FFT_NBANDS;++i) _fft_band[i]=0;

	_param=new Param();

	//read csv
	_csv_user_output.load(Param::val()->_csv_record);
	ofLog()<<"load csv data with "<<_csv_user_output.getNumRows()<<" rows!";


	_now_millis=ofGetElapsedTimeMillis();

	SceneBase::WinScale=max(ofGetWidth()/1920.0,ofGetHeight()/1080.0);
	SceneBase::SoundButton.load("sound/icon.wav");
	TextRunner::TextFont.loadFont("font/NotoSansCJKtc-Regular.otf",21);
	TextRunner::CharWid=TextRunner::TextFont.getFontSize()*1.4;

	ofDisableArbTex();
	_img_back.loadImage("ui/back.png");
	_img_back.getTextureReference().setTextureWrap(GL_MIRRORED_REPEAT,GL_MIRRORED_REPEAT);
	
	_show_keyboard=false;
	_keyboard=new PKeyboard(ofVec2f(450,720),ofVec2f(1020,340),18);


	loadScene();
	/*_mode=PEMPTY;
	setScene(PStatus::PHOME);*/

	_mode_pre=PEMPTY;
	_mode=PNAME;
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

	float tp=_now_millis/150000.0;
	_mesh_back.setTexCoord(0,ofVec2f(tp,0));
	_mesh_back.setTexCoord(1,ofVec2f(1+tp,0));
	_mesh_back.setTexCoord(2,ofVec2f(1+tp,1));
	_mesh_back.setTexCoord(3,ofVec2f(1+tp,1));
	_mesh_back.setTexCoord(4,ofVec2f(tp,1));
	_mesh_back.setTexCoord(5,ofVec2f(tp,0));

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetBackgroundColor(0);
	ofEnableAlphaBlending();

	ofEnableNormalizedTexCoords();
	_img_back.bind();
		_mesh_back.draw();
	_img_back.unbind();
	
	ofDisableNormalizedTexCoords();


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
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bool trigger_=_scene[_mode]->handleMousePressed(x,y);
	if(_show_keyboard){
		bool ktrigger_=_keyboard->checkMouse(ofPoint(x/SceneBase::WinScale,y/SceneBase::WinScale));
		if(!trigger_ && !ktrigger_) _show_keyboard=false;
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
	_show_keyboard=false;
	_keyboard->setLanguage(PKeyboard::PLANGUAGE::EN);

	_in_transition=true;

	
}

void ofApp::setSelectColor(int set_){
	_select_color=set_;
}
ofColor ofApp::getSelectColor(){
	return MainColor[_select_color];
}

void ofApp::setRecording(bool set_){
	
	if(set_){
		ofLog()<<"Start recording\n";

		_user_id=ofGetTimestampString("%Y%m%d%H%M%S");
		string pt=_param->_folder_export+_user_id+".wav";

		cout << pt<<"----\n";
		_recorder.setup(pt,SAMPLE_RATE,NUM_CHANNELS);
		_recorder.setFormat(SF_FORMAT_WAV | SF_FORMAT_PCM_16);
		
		_path_record=pt;	

	}else{
		ofLog()<<"Stop recording";		
		_recorder.finalize();
	
	
	}
	_recording=set_;

}
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
	
	if(_mode!=PRECORD) return;

	if(_recording)
		_recorder.addSamples(input,bufferSize*nChannels);
	
	calcVolume(input,bufferSize,nChannels);
	BmFFT::getSimpleSpectrumMono(BUFFER_SIZE, input, _fft_band);
	
}
void ofApp::calcVolume(float* data,int bufferSize,int nChannels){
	
	float cur_vol=0;
	for(int i=0;i<bufferSize;++i){
		if(nChannels==2){
			float left_=data[i*2]*0.5*Param::val()->_spectrum_scale;
			float right_=data[i*2+1]*0.5*Param::val()->_spectrum_scale;
			cur_vol+=left_*left_;
			cur_vol+=right_*right_;
		}else{
			cur_vol+=data[i]*Param::val()->_spectrum_scale;
		}
	}

	cur_vol/=(float)(bufferSize*nChannels);	
	//cur_vol=sqrt(cur_vol);
	//ofLog()<<cur_vol;


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
	_fft=set_;
}


void ofApp::setUserName(string set_){
	_user_name=set_;
}
void ofApp::setUserEmail(string set_){
	_user_email=set_;
}
void ofApp::setUserPhone(string set_){
	_user_phone=set_;
}
void ofApp::saveUserData(){
	
	ofxCsvRow row_;
	row_.addString(_user_id);
	row_.addInt(_select_color);
	row_.addString(_user_name);
	row_.addString(_user_email);
	row_.addString(_user_phone);

	cout<<"save data:"<<endl
		<<"id= "<<_user_id<<endl
		<<"user= "<<_user_name<<endl
		<<"email= "<<_user_email<<endl
		<<"phone= "<<_user_phone<<endl;

	_csv_user_output.addRow(row_);
	_csv_user_output.save();

}