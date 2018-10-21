#pragma once
#ifndef PDATABASE_H
#define PDATABASE_H

#include "ofMain.h"
#include "Parameter.h"
#include <sqlite_modern_cpp.h>

class PDatabase{
public:
	static string db_path;
	static string cin_path,map_path;		

	sqlite::database* _db;
	
	PDatabase(){
		_db=new sqlite::database(ofToDataPath(db_path));

	}
	static void generate(){

		sqlite::database gendb=sqlite::database(ofToDataPath(db_path));

		ofLog()<<"create db: "<<ofToDataPath(db_path);
		ofLog()<<"loading data...";

		gendb<<"DROP TABLE IF EXISTS pmap;";
		gendb<<"CREATE TABLE IF NOT EXISTS pmap (key TEXT, value TEXT);";

		// read mapping
		ofBuffer buffer1=ofBufferFromFile(map_path);
		for(auto line:buffer1.getLines()){
			auto text_=ofSplitString(line," ");

			if(text_.size()<3) continue;

			string key_=text_[0];
			string val_=text_[2];

			gendb<<u"INSERT INTO pmap (key,value) values (?,?);"
				<<key_<<val_;
		}


		gendb<<"DROP TABLE IF EXISTS p2c;";
		gendb<<"CREATE TABLE IF NOT EXISTS p2c (key TEXT, value TEXT);";

		// read words
		ofBuffer buffer=ofBufferFromFile(cin_path);
		for(auto line:buffer.getLines()){
			if(line[0]=='#' || line[0]=='%') continue;
			auto text_=ofSplitString(line,"	");
			
			if(text_.size()<2) continue;

			string key_=text_[0];
			string char_=text_[1];

			gendb<<u"INSERT INTO p2c (key,value) values (?,?);"
				<<key_<<char_;
		}
		ofLog()<<"Finish!";		
	}

	list<string> key2CWord(string key_){
		
		
		string en_key;
		//*_db<<u"SELECT value FROM pmap WHERE key=\""+u16string(key_.begin(),key_.end())+u"\";" 
		
		wstring ws(key_.begin(),key_.end());
		//u16string us=Param::utf82u16(key_);
		
		list<string> val;
		try{
		*_db<<"SELECT value FROM pmap WHERE key=\""+key_+"\";" 
			>>en_key;

		
		*_db<<"SELECT value FROM p2c WHERE key=\""+en_key+"\";" 
			>>[&](string s){
				if(s.length()>0){
					wstring ws=Param::utf82ws(s);
					string u8s=Param::ws2utf8(ws);
					if(u8s!="") val.push_back(s);					
				}
			};
		}catch(exception& e){
			ofLog()<<e.what();
		}
		return val;

	}




};


#endif
