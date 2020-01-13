
#ifndef _search_hpp_
#define _search_hpp_
#include<iostream>
#include<string>
#include<sqlite3.h>
#include<vector>
#include<locale>
#include<codecvt>
#include"test.hpp"
#include"str.hpp"
using namespace std;




struct token 
{
	int page_num;
	int index;
	string word;
	vector<int> page;
	vector<vector<int>> place;	
};




vector<int> body_search(int index1,vector<int> vec_index,vector<token> Token){
	vector<int> body_index(Token.size(),0),char_index;

	for(int i=0;i<(int)Token[0].place[index1].size();i++){
		int flag=0,place1=Token[0].place[index1][i]-Token[0].index;

		for(int j=1;j<(int)Token.size();j++){
			int place2=Token[j].place[vec_index[j]][body_index[j]]-Token[j].index;
			if(place1>place2){
				body_index[j]++;
				if(body_index[j]>=(int)Token[j].place[vec_index[j]].size())goto go;
				j--;
			}else if(place1<place2)break;
			else flag++;
		}

		if(flag==(int)Token.size()-1){
			char_index.push_back(place1);
		}
	}
	go:;
	return char_index;

}



void search_word(){
	wstring_convert<codecvt_utf8<char32_t>,char32_t> converter;
	vector<string> vec;//word
	vector<pair<string,string>> p_vec;//word,buff
	vector<pair<int,int>> s_vec;//num,index
	u32string s=U"";
	string word;
	cin>>word;
	u32string u32word=converter.from_bytes(word);

	int n=min(4,(int)u32word.size());//4文字に分割


	for(int i=0;i<n;i++)s+=u32word[i];
	vec.push_back(converter.to_bytes(s));


	for(int i=1;i<(int)u32word.size()-(n-1);i++){
		s.replace(0,1,U"");
		s+=u32word[i+(n-1)];
		vec.push_back(converter.to_bytes(s));
	}

	for(int i=0;i<(int)vec.size();i++){
		string please="SELECT*FROM DICK WHERE WORD=\'"+vec[i]+"\';";
		pair<int,const char*> p=numget(please);
		if(p.first==0){
			cout<<"Not exist"<<endl;
			return;
		}
		p_vec.push_back(make_pair(vec[i],p.second));
		s_vec.push_back(make_pair(p.first,i));
	}
	sort(s_vec.begin(),s_vec.end(),greater<pair<int,int>>());

	vector<token> Token;
	for(int i=0;i<(int)p_vec.size();i++){
		int ind=s_vec[i].second;
		vector<int> p;
		token t;

		//cout<<" num: "<<s_vec[i].first<<" index  :"<<ind<<" word : "<<p_vec[ind].first<<" buff :"<<p_vec[ind].second<<endl;		
		t.page_num=s_vec[i].first,t.index=ind , t.word=p_vec[ind].first;

		vector<vector<int>> v=decoder(p_vec[ind].second);
		for(int j=0;j<(int)v.size();j++){
			p.push_back(v[j][0]);
			v[j].erase(v[j].begin());
		}
		t.page=p;
		t.place=v;
		Token.push_back(t);
	}

	

	vector<int> vec_index(Token.size(),0);
	for(int i=0;i<(int)Token[0].page_num;i++){
		int page1=Token[0].page[i],flag=0;
		for(int j=1;j<(int)Token.size();j++){
			int page2=Token[j].page[vec_index[j]];
			if(page1>page2){
				vec_index[j]++;
				if(vec_index[j]>=Token[j].page_num)goto go;
				j--;
			}else if(page1<page2)break;
			else flag++;
		}
		


		if(flag==(int)Token.size()-1){
			vector<int> char_index=body_search(i,vec_index,Token);
			string please="SELECT*FROM BOOK WHERE ID="+to_string(page1)+";";
			output_page(please,char_index);
		}
	}
	go:;


}


#endif
