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

#include "peerserver.h"

using namespace std;

#define MAX_SIZE 1024

string username;

string si="127.0.0.1";
int sp=10000;

bool UserLogin(string username,string password,int myport_i){
	struct sockaddr_in remote_server;
	int sock;
	char output[MAX_SIZE];

	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(-1);
	}

	remote_server.sin_family=AF_INET;
	remote_server.sin_port=htons(sp);
	remote_server.sin_addr.s_addr=inet_addr(si.c_str());
	bzero(&remote_server.sin_zero,8);

	if((connect(sock,(struct sockaddr*)&remote_server,sizeof(struct sockaddr_in)))==-1){
		perror("connect");
		exit(-1);
	}

	string data="login "+username+" "+password+" "+to_string(myport_i);
	send(sock,data.c_str(),data.size(),0);
	int len=recv(sock,output,MAX_SIZE,0);
	output[len]='\0';
	// cout<<output<<endl;

	if(output[0]=='0')
		return false;
	else
		return true;
}

bool UserCreate(string username,string password,int myport_i){
	struct sockaddr_in remote_server;
	int sock;
	char output[MAX_SIZE];

	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(-1);
	}

	remote_server.sin_family=AF_INET;
	remote_server.sin_port=htons(sp);
	remote_server.sin_addr.s_addr=inet_addr(si.c_str());
	bzero(&remote_server.sin_zero,8);

	if((connect(sock,(struct sockaddr*)&remote_server,sizeof(struct sockaddr_in)))==-1){
		perror("connect");
		exit(-1);
	}

	string data="create_user "+username+" "+password+" "+to_string(myport_i);
	send(sock,data.c_str(),data.size(),0);
	int len=recv(sock,output,MAX_SIZE,0);
	output[len]='\0';
	cout<<output<<endl;

	if(output[0]=='0')
		return false;
	else
		return true;
}

bool GroupCreate(string groupname, string username){
	// cout<<groupname<<endl;
	// cout<<username<<endl;

	struct sockaddr_in remote_server;
	int sock;
	char output[MAX_SIZE];

	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(-1);
	}

	remote_server.sin_family=AF_INET;
	remote_server.sin_port=htons(sp);
	remote_server.sin_addr.s_addr=inet_addr(si.c_str());
	bzero(&remote_server.sin_zero,8);

	if((connect(sock,(struct sockaddr*)&remote_server,sizeof(struct sockaddr_in)))==-1){
		perror("connect");
		exit(-1);
	}

	string data="create_group "+groupname+" "+username;
	send(sock,data.c_str(),data.size(),0);
	int len=recv(sock,output,MAX_SIZE,0);
	output[len]='\0';
	cout<<output<<endl;

	if(output[0]=='0')
		return false;
	else
		return true;
}

void GroupList(){
	struct sockaddr_in remote_server;
	int sock;
	char output[MAX_SIZE];

	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(-1);
	}

	remote_server.sin_family=AF_INET;
	remote_server.sin_port=htons(sp);
	remote_server.sin_addr.s_addr=inet_addr(si.c_str());
	bzero(&remote_server.sin_zero,8);

	if((connect(sock,(struct sockaddr*)&remote_server,sizeof(struct sockaddr_in)))==-1){
		perror("connect");
		exit(-1);
	}

	string data="list_groups";
	send(sock,data.c_str(),data.size(),0);
	int len=recv(sock,output,MAX_SIZE,0);
	output[len]='\0';
	cout<<"##### List of Groups #####"<<endl<<endl;
	cout<<output<<endl;

	cout<<"#####  #####"<<endl;
}

bool Logout(string username){

	struct sockaddr_in remote_server;
	int sock;
	char output[MAX_SIZE];

	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(-1);
	}

	remote_server.sin_family=AF_INET;
	remote_server.sin_port=htons(sp);
	remote_server.sin_addr.s_addr=inet_addr(si.c_str());
	bzero(&remote_server.sin_zero,8);

	if((connect(sock,(struct sockaddr*)&remote_server,sizeof(struct sockaddr_in)))==-1){
		perror("connect");
		exit(-1);
	}

	string data="logout "+username;
	send(sock,data.c_str(),data.size(),0);
	int len=recv(sock,output,MAX_SIZE,0);
	output[len]='\0';
	cout<<output<<endl;

	if(output[0]=='0')
		return false;
	else
		return true;
}

int main(int argc, char** argv){

	bool flag=true;
	bool login_flag=false;
	string command;
	pthread_t tid;
	vector<string> command_split(10);

	string myport=argv[1];
	int myport_i=atoi(myport.c_str());
	string filepath=argv[2];

	auto pid=pthread_create(&tid,NULL,ClientServer,&myport_i);

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

			login_flag=UserLogin(command_split[1],command_split[2],myport_i);
			if(login_flag){
				cout<<"Successful"<<endl;
				username=command_split[1];
			}
			else{
				cout<<"Failed"<<endl;
			}
		}
		else if(command_split[0]=="create_user"){
			if(login_flag){
				cout<<"Already Logged in"<<endl;
				continue;
			}

			command_object>>command_split[1];
			command_object>>command_split[2];
			login_flag=UserCreate(command_split[1],command_split[2],myport_i);

			if(login_flag){
				cout<<"Successful"<<endl;
				username=command_split[1];
			}
			else{
				cout<<"Failed"<<endl;
			}			
		}
		else if(command_split[0]=="create_group"){
			if(!login_flag){
				cout<<"Log in first"<<endl;
				continue;
			}

			command_object>>command_split[1];

			if(GroupCreate(command_split[1],username)){
				cout<<"Successful"<<endl;
			}
			else{
				cout<<"Failed"<<endl;
			}
		}
		else if(command_split[0]=="list_groups"){
			if(!login_flag){
				cout<<"Log in first"<<endl;
				continue;
			}
			GroupList();
		}
		else if(command_split[0]=="logout"){
			if(!login_flag){
				cout<<"Log in first"<<endl;
				continue;
			}

			if(Logout(username)){
				cout<<"Successful"<<endl;
				login_flag=false;
			}
			else{
				cout<<"Failed"<<endl;
			}
		}
		else{
			if(!login_flag){
				cout<<"Log in first"<<endl;
				continue;
			}
			cout<<"Out"<<endl;
		}
	}

	return 0;
}