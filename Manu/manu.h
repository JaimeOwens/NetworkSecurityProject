#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;

#define OK 1
#define ERROR -1

void SplitString(string &srcStr, vector<string> &vec, string &separator){
	string::size_type posSubstringStart;
    string::size_type posSeparator;       
    posSeparator= srcStr.find(separator);
    posSubstringStart= 0;
    while (string::npos != posSeparator){
        vec.push_back(srcStr.substr(posSubstringStart, posSeparator- posSubstringStart));
        posSubstringStart= posSeparator+ separator.size();
        posSeparator= srcStr.find(separator, posSubstringStart);
    }
    if (posSubstringStart!= srcStr.length())  
        vec.push_back(srcStr.substr(posSubstringStart));
}

int InputItems(string shoppath){
	ofstream fout2(shoppath);
	if(!fout2){
		return ERROR;
	}
	int itemnumber;
	cout<<"3. Enter the number of items:"<<endl;
	cin>>itemnumber;
	int i;
	cout<<"4. Enter the name and price of item:"<<endl;
	for (i = 1; i <= itemnumber; i++){
		string itemname;
		string itemprice;
		cout<<"Item "<<i<<": ";
		cin>>itemname>>itemprice;
		fout2<<i<<' '<<itemname<<' '<<itemprice<<endl;
	}
	fout2.close();
	return OK;
}

int InputMenu(){
	cout<<"Welcome to manage the items."<<endl;
	int shopnumber;
	cout<<"1. Enter the number of shops:"<<endl;
	cin>>shopnumber;
	ofstream fout("itemlist/ShopList");
	if(!fout){
		return ERROR; 
	}
	int i;
	for(i=1; i<=shopnumber; i++){
		string shopname;
		cout<<"2. Enter the name of shop"<<endl;
		cin>>shopname;
		string shoppath;
		shoppath = "itemlist/" + shopname;
		fout<<i<<' ';
		fout<<shopname<<' ';
		fout<<shoppath<<endl;
		InputItems(shoppath);
	}
	fout.close();
	return OK;
}

int OutputItems(string shoppath){
	ifstream fin(shoppath);
	string temp;
	int count = 0;
	while(getline(fin, temp)){
		cout<<temp<<endl;
	}
	fin.close();
	return OK;
}

int OutputMenu(){
	ifstream fin("itemlist/ShopList");
	if(!fin){
		cout<<"can't open manu file"<<endl;
		return ERROR;
	}
	string temp;
	cout<<"**********ShopList**********"<<endl;
	while(getline(fin, temp)){
		vector<string> vec;
		string separator = " ";
		SplitString(temp, vec, separator);
		cout<<vec[0]<<' '<<vec[1]<<' '<<vec[2]<<endl;
		OutputItems(vec[2]);
	}
	fin.close();
	return OK;
}

string EnterPersonInfo(){
	string pi;
	if(OutputMenu()){
		string shop_name, item_id, number; 
		cout<<"Enter the shop's name , item id and item number"<<endl;
		cout<<"Enter shop's name equal to 0 to end"<<endl;
		while(cin>>shop_name){
			if(shop_name == "0"){
				break;
			}
			cin>>item_id>>number;
			pi += (shop_name + ' ' + item_id + ' ' + number + '\n');
		}
	}
	else{
		return "ERROR";
	}
	cout<<pi<<endl;
	return pi;
}

string EnterOwnerInfoByName(string shopname){
	string oi, temp; 
	string shoppath = "itemlist/" + shopname;
	ifstream fin(shoppath);
	if(!fin){
		return "ERROR";
	}
	while(getline(fin, temp)){
		oi += temp;
	}
	cout<<oi<<endl;
	return oi;
}




