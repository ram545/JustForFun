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
		return temp.substr(0,temp.length()-1);
	else
		return ret;
}

string getQuery(string query){
	string ret;
	int i;
	for(i=0;i<query.length();i++){
		if(query[i]=='.' || query[i]=='~'){
			ret = query.substr(0,i);
			break;
		}
		if(i==query.length()-1){
			ret = query;
			break;
		}
	}
	return ret;
}

HRML* CreateTagMap(vector<string> lst,int& N){
	HRML *tagTemp = new HRML;
	string temp,elmnt,attr;
	bool flag=false;
	vector<string> tokens;
	while(N<lst.size()){
		temp = lst[N];
		if(temp[1]=='/'){
			return tagTemp;
		}
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
	return tagTemp;
}

string solve(HRML* tags,string query){
	int i;
	string temp;
	bool flag;
	if(query[0]=='.')
		flag = true;
	else if(query[0]=='~')
		flag = false;
	else
		return "Not Found!";
	query = query.substr(1);
	for(i=0;i<query.length();i++){
		if(query[i]=='.' || query[i]=='~'){
			temp = query.substr(0,i);
			break;
		}
		if(i==query.length()-1){
			temp = query;
			break;
		}
	}
	if(flag){
		//cout << "tag: " << temp << endl;
		if(tags->nestTag.count(temp)==1)
			return solve(tags->nestTag[temp],query.substr(i));
		else	
			return "Not Found!";
	}
	else{
		//cout << "value: " << temp << endl;
		if(tags->attrib.count(temp)==1)
			return tags->attrib[temp];
		else
			return "Not Found!";
	}
}


int main(){
	int testCase,Query,N=0;
	string temp,temp1;	
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
	while(N<testCases.size()){
		temp = getTag(testCases[N]);
		cout << temp << endl;
		temp = temp.substr(1,temp.length()-1);
		cout << temp << endl;
		HRML *tagLst=CreateTagMap(testCases,N);
		tags[temp]=tagLst;
		N++;
	}
	N=0;
	while(N<queries.size()){
		temp = getQuery(queries[N]);
		if(tags.count(temp)>0)
			cout << solve(tags[temp],queries[N].substr(temp.length())) << endl;
		else
			cout << "Not Found!" << endl;
		N++;
	}
}

/*20 19
<tag1 v1 = "123" v2 = "43.4" v3 = "hello">
</tag1>
<tag2 v4 = "v2" name = "Tag2">
<tag3 v1 = "Hello" v2 = "World!">
</tag3>
<tag4 v1 = "Hello" v2 = "Universe!">
</tag4>
</tag2>
<tag5>
<tag7 new_val = "New">
</tag7>
</tag5>
<tag6>
<tag8 intval = "34" floatval = "9.845">
<ntag nv = "4$">
<nvtag nv = "6$">
</nvtag>
</ntag>
</tag8>
</tag6>
tag1~v1
tag1~v2
tag1~v3
tag4~v2
tag2.tag4~v1
tag2.tag4~v2
tag2.tag3~v2
tag5.tag7~new_val
tag5~new_val
tag7~new_val
tag6.tag8~intval
tag6.tag8~floatval
tag6.tag8~val
tag8~intval
tag6.tag8.ntag~nv
tag6.tag8.ntag.nvtag~nv
tag6.tag8.nvtag~nv
randomtag~nv
tag283.tag21.den~jef
*/

/*
6 4
<a>
<b name = "tag_one">
<c name = "tag_two" value = "val_907">
</c>
</b>
</a>
a.b~name
a.b.c~value
a.b.c~src
a.b.c.d~name
*/

/*
16 14
<tag1 v1 = "123" v2 = "43.4" v3 = "hello">
</tag1>
<tag2 v4 = "v2" name = "Tag2">
<tag3 v1 = "Hello" v2 = "World!">
</tag3>
<tag4 v1 = "Hello" v2 = "Universe!">
</tag4>
</tag2>
<tag5>
<tag7 new_val = "New">
</tag7>
</tag5>
<tag6>
<tag8 intval = "34" floatval = "9.845">
</tag8>
</tag6>
tag1~v1
tag1~v2
tag1~v3
tag4~v2
tag2.tag4~v1
tag2.tag4~v2
tag2.tag3~v2
tag5.tag7~new_val
tag5~new_val
tag7~new_val
tag6.tag8~intval
tag6.tag8~floatval
tag6.tag8~val
tag8~intval
*/
