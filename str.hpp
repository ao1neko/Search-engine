#ifndef _str_hpp_
#define _str_hpp_
#include<iostream>
#include<string>
#include<vector>
#include<locale>
#include<codecvt>
using namespace std;






vector<string> n_gram(u32string s,int n){
	wstring_convert<codecvt_utf8<char32_t>,char32_t> converter;
	vector<string> vec;
	u32string news=U"";
	for(int i=0;i<n;i++)news+=s[i];
	vec.push_back(converter.to_bytes(news));

	for(int i=1;i<(int)s.size()-(n-1);i++){
		news.replace(0,1,U"");
		news+=s[i+(n-1)];
		vec.push_back(converter.to_bytes(news));
	}

	return vec;
}

vector<vector<int>> decoder(string s){
	vector<vector<int>> vec;
	vector<int> v;
	string str="";
	for(int i=1;i<(int)s.size();i++){
		if(s[i]=='a'){
			v.push_back(stoi(str));
			str="";
			vec.push_back(v);
			v.clear();
		}else if(s[i]=='b'){
			v.push_back(stoi(str));
			str="";
		}else{
			str+=s[i];
		}	
	}
	v.push_back(stoi(str));
	vec.push_back(v);
	return vec;	
}





#endif
