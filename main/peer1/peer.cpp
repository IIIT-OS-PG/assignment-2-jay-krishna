#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <set>

#include "peerhelper.h"

using namespace std;

// bool UserLogin(string username,string password){
// 	struct sockaddr_in remote_server;
// 	int sock;
// 	char output[MAX_SIZE];

// 	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
// 		perror("socket");
// 		exit(-1);
// 	}

// 	remote_server.sin_family=AF_INET;
// 	remote_server.sin_port=htons(10000);
// 	remote_server.sin_addr.s_addr=inet_addr("127.0.0.1");
// 	bzero(&remote_server.sin_zero,8);

// 	if((connect(sock,(struct sockaddr*)&remote_server,sizeof(struct sockaddr_in)))==-1){
// 		perror("connect");
// 		exit(-1);
// 	}

// 	string data="login "+username+" "+password;
// 	send(sock,data.c_str(),data.size(),0);
// 	int len=recv(sock,output,MAX_SIZE,0);
// 	output[len]='\0';
// 	cout<<output<<endl;

// 	if(output[0]=='0')
// 		return false;
// 	else
// 		return true;

// 	return true;
// }

int main(int argc, char** argv){

	bool flag=true;
	bool login_flag=false;
	string command;
	vector<string> command_split(3);

	string myport=argv[1];
	int myport_i=atoi(myport.c_str());
	string filepath=argv[2];
	

	while(flag){
		getline(cin,command);
		stringstream command_object(command);
		command_object>>command_split[0];

		if(command_split[0]=="login"){
			if(login_flag){
				cout<<"Already Logged in"<<endl;
				continue;
			}
			command_object>>command_split[1];
			command_object>>command_split[2];

			login_flag=UserLogin(command_split[1],command_split[2]);
		}
		else if(command_split[0]=="create_user"){
			if(login_flag){
				cout<<"Already Logged in"<<endl;
				continue;
			}

			command_object>>command_split[1];
			command_object>>command_split[2];
			// UserCreate(command_split[1],command_split[2]);
		}
		else{
			if(login_flag){
				cout<<"Log in first"<<endl;
				continue;
			}
			cout<<"Out"<<endl;
		}
	}

	return 0;
}