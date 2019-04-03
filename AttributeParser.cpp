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
			return ret;
		}
	}
}

struct HRML* CreateTagMap(vector<string> lst,int& N){
	struct HRML *tagTemp = (struct HRML*)malloc(sizeof(struct HRML));
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
	int N=0;
	struct HRML *tagLst=CreateTagMap(testCases,N);
}
