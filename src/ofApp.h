#pragma once

#include "ofMain.h"
#include "ofxLibsndFileRecorder.h"

#include "FrameTimer.h"
#include "TextRunner.h"
#include "SceneBase.h"
#include "Parameter.h"


//#define DRAW_DEBUG

#define NUM_CHANNELS 1
#define SAMPLE_RATE 44100
#define BUFFER_SIZE 1024


class ofApp : public ofBaseApp{
		int _select_color;
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

		float _now_millis;

		bool _in_transition;
		void onSceneInFinish(int &e);
		void onSceneOutFinish(int &e);

		
		void setSelectColor(int set_);
		ofColor getSelectColor();


		void audioReceived(float * input, int bufferSize, int nChannels);
		bool recording;
		ofxLibsndFileRecorder audioRecorder;
};
