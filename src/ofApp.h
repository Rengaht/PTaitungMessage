#pragma once

#include "ofMain.h"
#include "ofxLibsndFileRecorder.h"
#include "ofxOsc.h"
#include "ofxFft.h"

#include "FrameTimer.h"
#include "TextRunner.h"
#include "SceneBase.h"
#include "Parameter.h"
#include "PKeyboard.h"

//#define DRAW_DEBUG

#define NUM_CHANNELS 2
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 256

#define FFT_NBANDS 42 //2*log2(buffer_size)



//#define SOUND_SCALE 500000
//#define SPECTRUM_BUFFER_SIZE 200

class ofApp : public ofBaseApp{
		float _now_millis;

		bool _recording;
		bool _use_fft;

		/* user info */
		int _select_color;
		string _path_record;
		string _user_id;
		wstring _user_name,_user_email,_user_phone;


		bool _show_keyboard;
		//keyboard
		PKeyboard* _keyboard;
	public:

		static ofColor MainColor[4];

		void setup();
		void exit();
		void update();
		void draw();
		void drawBackground();

		void keyReleased(int key);
		void mouseReleased(int x, int y, int button);
		
		enum PStatus {PHOME,PINTRO,PCOLOR,PNAME,PRECORD,PINFO,PEMPTY};
		PStatus _mode,_mode_pre;

		ofMesh _mesh_back;
		ofImage _img_back;


		SceneBase* _scene[6];
		void loadScene();
		void setScene(PStatus set_);


		//Param* _param;

	
		bool _in_transition;
		void onSceneInFinish(int &e);
		void onSceneOutFinish(int &e);

		
		void setSelectColor(int set_);
		int getSelectColor();

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
		ofxFft* _fft;
		float* audioInput;
		float* fftOutput;
		float* eqFunction;
		float* eqOutput;
		


		PKeyboard* getKeyboard();
		void setShowKeyboard(bool set_,PKeyboard::PLANGUAGE lan_=PKeyboard::PLANGUAGE::EN,bool show_language_=true);
		void updateKeyboardInput(wstring str_,int cursor_,int max_);
		
		
		void setUserName(wstring set_);
		void setUserEmail(wstring set_);
		void setUserPhone(wstring phone_);
		

		// for output
		//ofxCsv _csv_user_output;
		void saveUserData();


		void sendUpdateOsc();

		ofSerial _serial;
		void sendVolumeLight();
};
