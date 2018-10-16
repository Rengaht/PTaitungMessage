#pragma once

#ifndef SCENE_BASE_H
#define SCENE_BASE_H


#define DRAW_DEBUG


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
	void drawDebugInfo();
	virtual void init();
	virtual void end();
	virtual void update(float dt_);
	virtual void reset();

	void handleMousePressed(float mouse_x,float mouse_y);	
	virtual void buttonEvent(int index){}
	ofVec2f _mouse_pos;
	
	vector<ofRectangle> _button;

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

};


#endif