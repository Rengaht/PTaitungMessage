#pragma once
#ifndef PDATABASE_H
#define PDATABASE_H

#include "ofMain.h"
#include "ofxSQLiteHeaders.h"

class PDatabase{
public:
	static string db_path;
	static string cin_path;		
	static string table_name;

	ofxSQLite* _sqlite;

	PDatabase(){
		_sqlite=new ofxSQLite(db_path);
	}
	static void generate(){

		ofxSQLite* sqlite;

		sqlite=new ofxSQLite(db_path);
		ofLog()<<"create db: "<<db_path;

		string cmd_drop="DROP TABLE IF EXISTS "+table_name+";";
		sqlite->simpleQuery(cmd_drop.c_str());

		string cmd_create="CREATE TABLE IF NOT EXISTS "+table_name+" (id TEXT, value TEXT);";
		sqlite->simpleQuery(cmd_create.c_str());
		ofLog()<<cmd_create;

		ofBuffer buffer=ofBufferFromFile(cin_path);
		for(auto line:buffer.getLines()){
			if(line[0]=='#' || line[0]=='%') continue;
			auto text_=ofSplitString(line,"	");
			
			if(text_.size()<2) continue;

			string key_=text_[0];
			string char_=text_[1];

			string cmd_insert="INSERT INTO "+table_name+" VALUES (\""+key_+"\", \""+char_+"\");";
			ofLog()<<"insert "<<key_<<"-"<<char_;
			//sqlite->insert(table_name).use("id",key_).use("value",char_).execute();
			sqlite->simpleQuery(cmd_insert.c_str());

			//ofxSQLiteSelect sel=sqlite->select("value").from(table_name)//.where("id",key_)
			//	.execute().begin();
			//while(sel.hasNext()){
			//	ofLog()<<key_<<" -> "<<sel.getString();				
			//}

		}

		
	}

	string key2CWord(string key_){
		
		
		ofxSQLiteSelect sel=_sqlite->select("value").from(table_name).where("id",key_).limit(1)
									.execute().begin();
		while(sel.hasNext()){
			string w=sel.getString();
			ofLog()<<key_<<" -> "<<w;
			return w;
		}

	}




};


#endif
