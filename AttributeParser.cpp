#include<iostream>
#include<map>
#include<vector>
#include<cstring>
#include<iterator>
using namespace std;


struct HRML{
	string elmnt;
	map<string,string> attrib;
	map<string,HRML*> nestTag;
};

vector<string> stringParser(string temp){
	vector<string> tokens;
	int i,last=0,len=temp.length();
	for(i=0;i<len;i++){
		if(temp[i]==' '){
			tokens.push_back(temp.substr(last,i-last));
			last=i+1;
		}
		if(i==len-1 && last!=len){
			tokens.push_back(temp.substr(last,i+1-last));
			break;
		}
	}
	return tokens;
}

string getTag(string temp){
	string ret;
	int i,len=temp.length();
	for(i=0;i<len;i++){
		if(temp[i]==' '){
			ret=temp.substr(0,i);
			break;
		}
	}
	if(i==len)
		return temp;
	else
		return ret;
}

HRML* CreateTagMap(vector<string> lst,int& N){
	HRML *tagTemp = new HRML;
	string temp,elmnt,attr;
	bool flag=false;
	vector<string> tokens;
	while(N<lst.size()){
		temp = lst[N];
		if(temp[1]=='/')
			return tagTemp;
		else{
			elmnt = getTag(temp.substr(1,temp.length()-2));
			if(tagTemp->elmnt.length()<1){
				tagTemp->elmnt=elmnt;
				tokens = stringParser(temp.substr(1,temp.length()-2));
				vector<string>:: iterator it;
				it = tokens.begin();
				tokens.erase(it);
				for(it=tokens.begin();it!=tokens.end();it++){
					temp = *it;
					if(temp[0]!='='){
						if(!flag){
							attr = temp;
							flag = true;
						}
						else{
							temp = temp.substr(1,temp.length()-2);
							tagTemp->attrib.insert({attr,temp});
							flag = false;
						}
					}
				}
			}
			else{
				tagTemp->nestTag[elmnt]=CreateTagMap(lst,N);
			}
		}
		tokens.clear();
		N++;
	}
}

void solve(map<string,HRML*> tags,string query){
	int i;
	string temp;
	bool flag;
	for(i=0;i<query.length();i++){
		if(query[i]=='.' || query[i]=='~'){
			temp = query.substr(0,i);
			flag=true;
			break;
		}
		if(i==query.length()-1){
			temp = query;
			flag = false;
			break;
		}
	}
	if(flag){
		cout << "tag: " << temp << endl;
 		solve(tags,query.substr(i+1));
	}
	else{
		cout << "value: " << temp << endl;
	}
}


int main(){
	int testCase,Query,N=0;
	string temp;	
	vector<string> testCases;
	vector<string> queries;
	map<string,HRML*> tags;
	cin >> testCase >> Query;
	cin.ignore(250,'\n');
	while(testCase--){
		getline(cin,temp);
		testCases.push_back(temp);
	}
	while(Query--){
		getline(cin,temp);
		queries.push_back(temp);
	}
	map<string,string>::iterator it;
	while(N<testCases.size()){
		temp = getTag(testCases[N]);
		temp = temp.substr(1,temp.length()-1);
		HRML *tagLst=CreateTagMap(testCases,N);
		tags[temp]=tagLst;
		N++;
	}
	N=0;
	while(N<queries.size()){
		solve(tags,queries[N]);
		N++;
	}
}
