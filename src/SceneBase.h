#pragma once

#ifndef SCENE_BASE_H
#define SCENE_BASE_H




#include "ofMain.h"
#include "FrameTimer.h"

class ofApp;

class SceneBase{

public:
	static float WinScale;

	static float EaseDue;
	static float EaseDelay;
	static float EaseDistort;

	enum SceneStatus {Init, Due, End};
	SceneStatus _status;

	ofApp* _ptr_app;
	int _order_scene;

	
	SceneBase(ofApp *set_ptr);
	virtual ~SceneBase();

	virtual void loadImage(){}
	virtual void setupTimer();
	void setup();

	void drawScaled(bool debug_=false);
	virtual void draw();
	virtual void drawLayer(int i);
	virtual void drawDebugInfo();

	list<int> _zindex;

	virtual void init();
	virtual void end();
	virtual void update(float dt_);
	virtual void reset();

	virtual bool handleMousePressed(float mouse_x,float mouse_y);	
	virtual void buttonEvent(int index){}
	ofVec2f _mouse_pos;
	
	vector<ofRectangle> _button;
	vector<bool> _enable_button;

	int _mlayer;
	vector<FrameTimer> _timer_in;
	vector<FrameTimer> _timer_out;
	vector<ofImage> _img_ui;

	bool _trigger_in;
	bool _trigger_out;

	static ofEvent<int> sceneInFinish;
	static ofEvent<int> sceneOutFinish;

	//static ofSoundPlayer SoundIcon;
	static ofSoundPlayer SoundButton;
	static FrameTimer _timer_sleep;
	
	static ofImage* ImgPillLeft;
	static ofImage* ImgPillRight;

	void drawPillLeft(float x1,float hei,int index_,float alpha_=1.0);
	void drawPillRight(float x1,float hei,int index_,float alpha_=1.0);
};


#endif