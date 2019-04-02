https://www.hackerrank.com/challenges/attribute-parser/problem
#include<iostream>
#include<map>
#include<vector>
#include<cstring>
#include<iterator>
using namespace std;


struct HRML{
	string tag;
	map<string,string> attrib;
	map<string,struct HRML> nestTag;
};

struct HRML* CreateTagMap(vector<string> lst,int N){
	struct HRML *tagTemp = (struct HRML*)malloc(sizeof(struct HRML));
	string temp = lst[N];
	temp = temp.substr(1,temp.length());
	if(temp[0]!='/'){
		 
	}
}


int main(){
	int testCase,Query;
	string temp;	
	vector<string> testCases;
	vector<string> queries;
	cin >> testCase >> Query;
	cin.ignore(250,'\n');
	while(testCase--){
		cin >> temp;
		getline(cin,temp);
		testCases.push_back(temp);
	}
	while(Query--){
		cin >> temp;
		getline(cin,temp);
		queries.push_back(temp);
	}
	struct HRML *tag=CreateTagMap(testCases,0);
}
