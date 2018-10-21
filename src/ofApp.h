#pragma once

#include "ofMain.h"
#include "ofxLibsndFileRecorder.h"
#include "bmFFT.h"
#include "ofxCsv.h"

#include "FrameTimer.h"
#include "TextRunner.h"
#include "SceneBase.h"
#include "Parameter.h"
#include "PKeyboard.h"

#define DRAW_DEBUG

#define NUM_CHANNELS 1
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 256

#define FFT_NBANDS 16

//#define SOUND_SCALE 500000
//#define SPECTRUM_BUFFER_SIZE 200

class ofApp : public ofBaseApp{
		float _now_millis;

		bool _recording;
		bool _fft;

		/* user info */
		int _select_color;
		string _path_record;
		string _user_id;
		string _user_name,_user_email,_user_phone;

	public:

		static ofColor MainColor[4];

		void setup();
		void update();
		void draw();

		void keyReleased(int key);
		void mouseReleased(int x, int y, int button);
		
		enum PStatus {PHOME,PINTRO,PCOLOR,PNAME,PRECORD,PINFO,PEMPTY};
		PStatus _mode,_mode_pre;

		ofMesh _mesh_back;
		ofImage _img_back;


		SceneBase* _scene[6];
		void loadScene();
		void setScene(PStatus set_);


		Param* _param;

	
		bool _in_transition;
		void onSceneInFinish(int &e);
		void onSceneOutFinish(int &e);

		
		void setSelectColor(int set_);
		ofColor getSelectColor();

		ofSoundStream _sound_stream;
		void audioIn(float * input, int bufferSize, int nChannels); 

		void calcVolume(float *data,int bufferSize,int nchannels);
		float _volume_now;
		

		void setRecording(bool set_);
		ofxLibsndFileRecorder _recorder;
		

		ofSoundPlayer _player_record;
		void playRecord();


		// for fft
		void setFFT(bool set_);
		float* _fft_band;


		//keyboard
		bool _show_keyboard;
		PKeyboard* _keyboard;
		
		void setUserName(string set_);
		void setUserEmail(string set_);
		void setUserPhone(string phone_);
		

		// for output
		ofxCsv _csv_user_output;
		void saveUserData();
};
