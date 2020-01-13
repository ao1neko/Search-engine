#ifndef _test_hpp_
#define _test_hpp_
#include<iostream>
#include<string>
#include<sqlite3.h>
#include<fstream>
#include<map>
#include<regex>
#include<locale>
#include<codecvt>
#include<list>
#include<vector>
#include<iterator>
#include "str.hpp"
#include"new_map.hpp"
//#include"CppSQLite.h"
#include<ctime>
using namespace std;


static int callback(void *NotUsed,int column_count,char **data,char **column_name){
	int i;
	cout<<"------------------------------------"<<endl;
	for(i=0;i<column_count;i++)cout<<column_name[i]<<" , "<<data[i]<<endl;
	return 0;
}



void make_sql_table(char command){
	sqlite3 *db;
	int sql_status;
	char sql[1000]="";
	char *zero=0;

	sql_status=sqlite3_open("test.db",&db);
	if(sql_status)cout<<"Can't open db"<<endl;


	if(command=='t'){
		//make table
		strncpy(sql,"CREATE TABLE IF NOT EXISTS BOOK(ID INT PRIMARY KEY,TITLE TEXT NOT NULL,BODY TEXT NOT NULL);",sizeof(sql)-1);
		sql_status=sqlite3_exec(db,sql,callback,0,&zero);
		cout<<"make sql table"<<endl;
	}else if(command=='d'){
		//make table
		strncpy(sql,"CREATE TABLE IF NOT EXISTS DICK(WORD TEXT PRIMARY KEY,NUM INT NOT NULL,BUFF TEXT NOT NULL);",sizeof(sql)-1);
		sql_status=sqlite3_exec(db,sql,callback,0,&zero);
	}	
	if(sql_status!=SQLITE_OK){
		cout<<"SQL error at make sql"<<endl;
	}
	sqlite3_close(db);
}


void file_read(char command){
	sqlite3 *db;

	int sql_status,id_count=1;
	char sql[100000]="";
	char *zero=0;
	sql_status=sqlite3_open("test.db",&db);
	if(sql_status)cout<<"Can't open db"<<endl;

	ifstream ifs("./wikidata/wiki_01");
	if(ifs.fail())cout<<"File reading failed"<<endl;
	
	string wiki_string_line,news="",title;


	if(command=='i'){
		while(getline(ifs,wiki_string_line)){
			if(wiki_string_line[0]=='<'&&wiki_string_line[1]=='d'){
				smatch m;
				regex_search(wiki_string_line,m,regex("title=\".*\""));
				title=m.str().replace(0,7,"");
				title.erase(--title.end());
			}else if(wiki_string_line[0]=='<'){
				string sql_string="INSERT INTO BOOK(ID,TITLE,BODY) VALUES("+to_string(id_count)+",\'"+title+"\',\'"+news+"\');";
				strncpy(sql,sql_string.c_str(),sizeof(sql)-1);
				sql_status=sqlite3_exec(db,sql,callback,0,&zero);
				if(sql_status!=SQLITE_OK)cout<<"sql error2 at the insert"<<endl;
				news="";
				id_count++;
			}else{
				news+=wiki_string_line;
			}
		}
		cout<<"insert into sql"<<endl;
	}else if(command=='c'){
		strncpy(sql,"SELECT*FROM BOOK",sizeof(sql)-1);
		sql_status=sqlite3_exec(db,sql,callback,0,&zero);
		if(sql_status!=SQLITE_OK)cout<<"sql error2"<<endl;
	}else if(command=='l'){
		strncpy(sql,"SELECT*FROM DICK",sizeof(sql)-1);
		sql_status=sqlite3_exec(db,sql,callback,0,&zero);
		if(sql_status!=SQLITE_OK)cout<<"sql error2"<<endl;
	}

	sqlite3_close(db);
}



void insert_map(string s1,int c,string s2,sqlite3 *db){
	int sql_status;
	char sql[1000000]="";
	char *zero=0;

        string sql_string="INSERT INTO DICK(WORD,NUM,BUFF) VALUES(\'"+s1+"\',"+to_string(c)+",\'"+s2+"\');";
	//cout<<sql_string<<endl;
	strncpy(sql,sql_string.c_str(),sizeof(sql)-1);
	sql_status=sqlite3_exec(db,sql,callback,0,&zero);
	if(sql_status!=SQLITE_OK)cout<<"sql error3 at the insert"<<endl;

}


void sql_get(){
	sqlite3 *db;
	sqlite3_stmt *pstmt=NULL;
	wstring_convert<codecvt_utf8<char32_t>,char32_t> converter;
	int err=0;
	int sql_status;
	multimap<string,pair<int,int>> mp;


	sql_status=sqlite3_open("test.db",&db);
	if(sql_status)cout<<"Can't open db"<<endl;
	
	
	err=sqlite3_prepare_v2(db,"SELECT*FROM BOOK",64,&pstmt,NULL);
	if(err != SQLITE_OK){
		cout<<"sql error3"<<endl;
	}else{
		while(SQLITE_ROW==(err=sqlite3_step(pstmt))){
			int id=sqlite3_column_int(pstmt,0);
			const char *title=(const char*)sqlite3_column_text(pstmt,2);//1に戻すとtitle
			//const char *body=(const char*)sqlite3_column_text(pstmt,2);
			u32string s32=converter.from_bytes(title);//今はタイトルしか使いません
			vector<string> vec;
			vec=n_gram(s32,min((int)s32.size(),4));//4文字で分割
			for(int i=0;i<(int)vec.size();i++){
				mp.insert(make_pair(vec[i],make_pair(id,i)));
			}
		}
	}
	map<string,pair<int,string>> new_map=map_converter(mp);
	for(auto itr=new_map.begin();itr!=new_map.end();++itr){
		insert_map(itr->first,itr->second.first,itr->second.second,db);
	}

	sqlite3_close(db);
	sqlite3_finalize(pstmt);

	cout<<"make dictionary"<<endl;
}



pair<int,const char*> numget(string s){

	sqlite3 *db;
	sqlite3_stmt *pstmt=NULL;
	wstring_convert<codecvt_utf8<char32_t>,char32_t> converter;
	int err=0,num;
	int sql_status;
	const char *buff;


	sql_status=sqlite3_open("test.db",&db);
	if(sql_status)cout<<"Can't open db"<<endl;
	
	
	err=sqlite3_prepare_v2(db,s.c_str(),64,&pstmt,NULL);
	if(err != SQLITE_OK){
		cout<<"sql error3"<<endl;
	}else{
		while(SQLITE_ROW==(err=sqlite3_step(pstmt))){
			num=sqlite3_column_int(pstmt,1);
			buff=(const char*)sqlite3_column_text(pstmt,2);
		}
	}
	sqlite3_close(db);
	sqlite3_finalize(pstmt);

	return make_pair(num,buff);
}



void output_page(string s,vector<int> vec){
	sqlite3 *db;
	int sql_status;
	char *zero=0;
	sql_status=sqlite3_open("test.db",&db);
	if(sql_status)cout<<"Can't open db"<<endl;
cout<<s<<endl;	
	sql_status=sqlite3_exec(db,s.c_str(),callback,0,&zero);
	if(sql_status!=SQLITE_OK)cout<<"sql error2"<<endl;

	sqlite3_close(db);

	cout<<"word index :";
	for(int i=0;i<(int)vec.size();i++){
		cout<<vec[i]<<" ";
	}
	cout<<endl;

}


#endif



