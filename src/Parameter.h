﻿#pragma once
#ifndef PARMAMETR_H
#define PARAMETER_H

#include <codecvt>

#include "ofMain.h"
#include "ofxXmlSettings.h"


class Param{

	static Param* _instance;	
	
public:
	static string ParamFilePath;	
    
	
	string _folder_export;
	string _name_export;
	string _csv_record;
	
	vector<wstring> _question;
	int _index_question;

	int _time_record;
	int _time_sleep;

	float _spectrum_scale;
	float _spectrum_size;

	string _osc_address;
	int _osc_port;

	int _repeat_record;
	int _arduino_device;

	float _mic_low_thres;
	float _mic_scale;

	Param(){
		readParam();

	}
	static Param* val(){
		if(!_instance)
			_instance=new Param();
		return _instance;
	}
	void readParam(){

		_question.clear();

		ofxXmlSettings _param;
		bool file_exist=true;
		if(_param.loadFile(ParamFilePath) ){
			ofLog()<<ParamFilePath<<" loaded!";
		}else{
			ofLog()<<"Unable to load xml check data/ folder";
			file_exist=false;
		}


		int mquestion=_param.getNumTags("QUESTION");
		for(int i=0;i<mquestion;++i){
			_question.push_back(utf82ws(_param.getValue("QUESTION","",i)));
		}
		random_shuffle(_question.begin(),_question.end());
		_index_question=0;

        
		_folder_export=_param.getValue("FOLDER_EXPORT","");
		_name_export=_param.getValue("NAME_EXPORT","");
		_csv_record=_param.getValue("CSV_RECORD","");

		_time_record=_param.getValue("TIME_RECORD",0);
		_time_sleep=_param.getValue("TIME_SLEEP",0);

		_spectrum_scale=_param.getValue("SPECTRUM_SCALE",500000.0);
		_spectrum_size=_param.getValue("SPECTRUM_SIZE",200.0);

		_osc_address=_param.getValue("OSC_ADDRESS","");
		_osc_port=_param.getValue("OSC_PORT",12345);

		_repeat_record=_param.getValue("REPEAT_RECORD",1);
		_arduino_device=_param.getValue("ARDUINO_DEVICE",0);

		_mic_scale=_param.getValue("MIC_SCALE",2.0);
		_mic_low_thres=_param.getValue("MIC_LOW_THRES",50.0);

		if(!file_exist) saveParameterFile();

	
	}
	static string ws2utf8(std::wstring &input){
		
		try{
			static std::locale loc("");
			auto &facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
			return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).to_bytes(input);
		}catch(exception& e){
			cout<<e.what();
		}
		return "";
	}

	static wstring utf82ws(std::string &input)
	{
		try{
			std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8conv;
			return utf8conv.from_bytes(input);
		}catch(exception& e){
			cout<<e.what();
		}
		return wstring();
	}
	
	static string ws2utf8Output(wstring& input){
		std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8conv;
		return utf8conv.to_bytes(input);
	}
	void saveParameterFile(){


		ofxXmlSettings _param;
      
  
		for(int i=0;i<_question.size();++i){
			_param.setValue("QUESTION",ws2utf8(_question[i]),i);						
		}
		
		_param.setValue("FOLDER_EXPORT",_folder_export);
		_param.setValue("NAME_EXPORT",_name_export);
		_param.setValue("CSV_RECORD",_csv_record);

		_param.setValue("TIME_RECORD",_time_record);
		_param.setValue("TIME_SLEEP",_time_sleep);

		_param.setValue("SPECTRUM_SIZE",_spectrum_size);
		_param.setValue("SPECTRUM_SCALE",_spectrum_scale);

		_param.setValue("OSC_ADDRESS",_osc_address);
		_param.setValue("OSC_PORT",_osc_port);
		_param.setValue("REPEAT_RECORD",_repeat_record);

		_param.setValue("ARDUINO_DEVICE",_arduino_device);
		_param.setValue("MIC_SCALE",_mic_scale);
		_param.setValue("MIC_LOW_THRES",_mic_low_thres);

		_param.save(ParamFilePath);


	}
	wstring getRandomQuestion(){
			
		(++_index_question)%=_question.size();
		if(_index_question==0) random_shuffle(_question.begin(),_question.end());

		return _question[_index_question];
	}
	
};





#endif

