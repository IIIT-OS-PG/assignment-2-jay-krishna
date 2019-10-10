#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(){
// 	string obj="1";
// 	ofstream outfile("data.txt",ios::out);
// 	for(int j=1;j<=5;++j){
// 		obj=to_string(j);
// 	for(int i=0;i<1024;++i){
// 		if(i==1023)
// 			obj="0";
// 		outfile<<obj;
// 	}
// }
// 	outfile.close();
	char out[10240];
	FILE* f=fopen("data.txt","rb");
	int bt_s=fread(out,1,10*1024,f);
	out[bt_s]='\0';
	cout<<out<<endl<<endl;
	cout<<bt_s<<endl;
	fclose(f);






	return 0;
}