#pragma once
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
	vector<wstring> _question;
	int _index_question;

	int _time_record;

	float _spectrum_scale;
	float _spectrum_size;

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

		_time_record=_param.getValue("TIME_RECORD",0);
		_spectrum_scale=_param.getValue("SPECTRUM_SCALE",500000.0);
		_spectrum_size=_param.getValue("SPECTRUM_SIZE",200.0);

		if(!file_exist) saveParameterFile();

	
	}
	static string ws2utf8(std::wstring &input){
		/*std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8conv;
		return utf8conv.to_bytes(input);*/

		static std::locale loc("");
		auto &facet = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(loc);
		return std::wstring_convert<std::remove_reference<decltype(facet)>::type, wchar_t>(&facet).to_bytes(input);

	}

	static wstring utf82ws(std::string &input)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8conv;
		return utf8conv.from_bytes(input);
	}

	void saveParameterFile(){


		ofxXmlSettings _param;
      
  
		for(int i=0;i<_question.size();++i){
			_param.setValue("QUESTION",ws2utf8(_question[i]),i);						
		}
		
		_param.setValue("FOLDER_EXPORT",_folder_export);
		_param.setValue("NAME_EXPORT",_name_export);

		_param.setValue("TIME_RECORD",_time_record);
		_param.setValue("SPECTRUM_SIZE",_spectrum_size);
		_param.setValue("SPECTRUM_SCALE",_spectrum_scale);
		
		_param.save(ParamFilePath);


	}
	wstring getRandomQuestion(){
			
		(++_index_question)%=_question.size();
		if(_index_question==0) random_shuffle(_question.begin(),_question.end());

		return _question[_index_question];
	}

};





#endif

