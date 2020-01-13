#include<iostream>
#include<sqlite3.h>
#include<string>
#include<regex>
#include<list>
#include<iterator>
#include"search.hpp"
#include"test.hpp"

using namespace std;
int main(){
	char command;
	cout<<"please command"<<endl;
	cout<<endl;
	cout<<"t: make sql table"<<endl;
	cout<<"i: insert data"<<endl;
	cout<<"c: cout data"<<endl;
	cout<<"d: make sql table for dictionary"<<endl;
	cout<<"g; create dictionary"<<endl;
	cout<<"l: cout dictionary"<<endl;
	cout<<endl;
	cout<<"s: search word"<<endl;
	cin>>command;

	//準備
	make_sql_table(command);
	file_read(command);	
	if(command=='g')sql_get();


	//検索	
	if(command=='s')search_word();	

	cout<<"finish"<<endl;
	return 0;
}
