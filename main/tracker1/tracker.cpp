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

#define MAX_SIZE 1024

using namespace std;

unordered_map<string,string> credentials;
set<string> online;
vector<pair<string,string>> tracker_info;

// void FetchDetails(){
// 	credentials.clear();
// 	online.clear();
// 	string line,line1,line2;
	
// 	ifstream infile(".credential.txt",ios::in);
// 	if(infile){
// 		while(getline(infile,line)){
// 			stringstream line_object(line);
// 			line_object>>line1;
// 			line_object>>line2;

// 			credentials[line1]=line2;
// 		}
// 	}
// 	else{
// 		infile.close();
// 		ofstream outfile(".credential.txt",ios::out);
// 		outfile.close();
// 	}

// 	infile.open(".online.txt",ios::in);
// 	if(infile){
// 		while(getline(infile,line)){
// 			online.insert(line);
// 		}
// 	}
// 	else{
// 		infile.close();
// 		ofstream outfile(".online.txt",ios::out);
// 		outfile.close();
// 	}
// }

// void *TrackerKernel(void *pointer){
// 	int data_len;
// 	char data[MAX_SIZE+1];
// 	vector<string> command_split(3);
// 	int new_cli=*((int *)pointer);

// 	data_len=recv(new_cli,data,MAX_SIZE,0);
// 	data[data_len]='\0';
// 	cout<<data<<endl;
// 	cout<<data_len<<endl;

// 	string command(data);
// 	stringstream command_object(command);
// 	command_object>>command_split[0];

// 	if(command_split[0]=="login"){
// 		command_object>>command_split[1];
// 		command_object>>command_split[2];

// 		cout<<"server "<<command_split[1]<<" "<<command_split[2]<<endl;

// 		FetchDetails();

// 		if(credentials.find(command_split[1])==credentials.end()){
// 			send(new_cli,"0",1,0);
// 		}
// 		else{
// 			if(command_split[2]==credentials[command_split[1]]){
// 				send(new_cli,"1",1,0);

// 				if(online.find(command_split[1])==online.end()){
// 					online.insert(command_split[1]);

// 					ofstream outfile(".online.txt",ios::out|ios::app);
// 					outfile<<command_split[1];
// 					outfile<<endl;
// 				}

// 			}
// 			else
// 				send(new_cli,"0",1,0);
// 		}
// 	}

// 	return NULL;
// }

// void *TrackerServer(void *pointer){
// 	int mysequence_i=*((int*) pointer);
// 	struct sockaddr_in server,client;
// 	int sock,new_cli;
// 	pthread_t tid;

// 	int sockaddr_len=sizeof(struct sockaddr_in);
// 	socklen_t len=sizeof(struct sockaddr_in);
// 	string my_port=tracker_info[mysequence_i].second;
	
// 	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
// 		perror("server socket: ");
// 		exit(-1);
// 	}

// 	server.sin_family=AF_INET;
// 	server.sin_port=htons(atoi((tracker_info[mysequence_i].second).c_str()));
// 	server.sin_addr.s_addr=INADDR_ANY;
// 	bzero(&server.sin_zero,8);

// 	if((bind(sock,(struct sockaddr*)&server,sockaddr_len))==-1){
// 		perror("Bind");
// 		exit(-1);
// 	}

// 	if(listen(sock,5)==-1){
// 		perror("Listen");
// 		exit(-1);
// 	}

// 	cout<<my_port<<endl;
// 	while(true){
// 		if((new_cli=accept(sock,(struct sockaddr*)&client,&len))==-1){
// 			perror("accept");
// 			exit(-1);
// 		}
// 		auto pid=pthread_create(&tid,NULL,TrackerKernel,&new_cli);
// 		if(pid!=0){
// 			perror("thread failed");
// 			exit(-1);
// 		}
// 	}
// }

int main(int argc, char** argv){

	bool flag=true;
	string command,line,temp_ip,temp_port;
	pthread_t tid;
	int mysequence_i;

	string filepath=argv[1];
	string mysequence=argv[2];
	mysequence_i=atoi(mysequence.c_str())-1;

	ifstream infile;
	infile.open(filepath,ios::in);

	while(getline(infile,line)){
		cout<<line<<endl;
		stringstream line_object(line);
		line_object>>temp_ip;
		line_object>>temp_port;
		tracker_info.push_back(make_pair(temp_ip,temp_port));
	}

	FetchDetails();

	auto pid=pthread_create(&tid,NULL,TrackerServer,&mysequence_i);
	if(pid!=0){
		perror("Thread creation failed");
		exit(-1);
	}

	while(flag){
		cin>>command;
		if(command=="quit"){
			flag=false;
		}
	}

	pthread_cancel(tid);
	return 0;
}