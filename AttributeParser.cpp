#include<iostream>
#include<map>
#include<vector>
#include<cstring>
#include<iterator>
using namespace std;


struct HRML{
	string elmnt;
	map<string,string> attrib;
	map<string,struct HRML*> nestTag;
};

vector<string> stringParser(string temp){
	vector<string> tokens;
	int i,last=0,len=temp.length();
	for(i=0;i<len;i++){
		if(temp[i]=='32'){
			tokens.push_back(temp.substr(last,i-1));
			last=i+1;
		}
		if(i==len-1 && last!=len){
			tokens.push_back(temp.substr(last,len));
			break;
		}
	}
	return tokens;
}

string getTag(string temp){
	string ret;
	int i,len=temp.length();
	for(i=0;i<len;i++)
		if(temp[i]=='32'){
			ret=temp.substr(0,i-1);
			return ret;
		}
}

struct HRML* CreateTagMap(vector<string> lst,int N){
	struct HRML *tagTemp = (struct HRML*)malloc(sizeof(struct HRML));
	string temp,elmnt;
	vector<string> tokens;
	while(N<lst.size()){
		temp = lst[N];
		if(temp[1]=='/')
			return tagTemp;
		else{
			elmnt = getTag(temp.substr(1,temp.length()-1));
			if(tagTemp->elmnt.length()<1){
				tagTemp->elmnt=elmnt;
				tokens = stringParser(temp.substr(temp.length()-1));
				vector<string>:: iterator it;
				for(it=tokens.begin(),it++;it!=tokens.end();){
					tagTemp->attrib[*(it)]=*(it+2);
					it+=3;
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


int main(){
	int testCase,Query;
	string temp;	
	vector<string> testCases;
	vector<string> queries;
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
	struct HRML *tagLst=CreateTagMap(testCases,0);
}
