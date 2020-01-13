#ifndef _new_map_hpp_
#define _new_map_hpp_
#include<iostream>
#include<string>
#include<map>
using namespace std;


map<string,pair<int,string>> map_converter(multimap<string,pair<int,int>> mp){
	map<string,pair<int,string>> new_map;
	auto it=mp.begin();
	int count=1;
	int page=it->second.first;
	string befor_key=it->first,s="a"+to_string(page);


	for(auto itr=mp.begin();itr!=mp.end();itr++){	
	
		if(befor_key!=itr->first){
			new_map.insert(make_pair(befor_key,make_pair(count,s)));
			befor_key=itr->first;
			page=itr->second.first;
			s="a"+to_string(page);
			count=1;

		}
		else if(page!=itr->second.first){
			page=itr->second.first;
			s+="a"+to_string(page);
			count++;
		}
		
		s+="b"+to_string(itr->second.second);
	}
	new_map.insert(make_pair(befor_key,make_pair(count,s)));
	return new_map;
}

#endif
