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
unordered_map<string,pair<string,string> >online_users;
set<string> online;
unordered_map<string,string> groups;
vector<pair<string,string>> tracker_info;

struct Message{
	int new_cli;
	struct sockaddr_in client;
	string client_ip;
	string client_port;
};

vector<string> split(string data,char delim){
	vector<string> v;
	string temp="";
	for(long long unsigned int i=0;i<data.size();++i){
		if(data[i]==delim){
			v.push_back(temp);
			temp="";
		}
		else{
			// const char x=data[i];
			// cout<<x<<endl;
			temp.append(data,i,1);
			// cout<<temp<<endl;
		}
	}
	v.push_back(temp);

	return v;
}

void DeleteLine(string filename,string data,int index){
	string line;
	ifstream infile(filename,ios::in);
	ofstream outfile(".temp.txt",ios::out|ios::app);

	while(getline(infile,line)){
			vector<string> v=split(line,' ');
			if(v[index]==data)
				continue;
			else{
				outfile<<line<<endl;
			}
	}

	remove(filename.c_str());
	rename(".temp.txt",filename.c_str());
	infile.close();
}

void FetchDetails(){
	credentials.clear();
	online.clear();
	online_users.clear();
	string line,line1,line2;
	
	ifstream infile(".credential.txt",ios::in);
	if(infile){
		while(getline(infile,line)){
			stringstream line_object(line);
			line_object>>line1;
			line_object>>line2;

			credentials[line1]=line2;
		}
		infile.close();
	}
	else{
		infile.close();
		ofstream outfile(".credential.txt",ios::out);
		outfile.close();
	}

	infile.open(".online.txt",ios::in);
	if(infile){
		while(getline(infile,line)){
			vector<string> split_vector=split(line,' ');
			online.insert(split_vector[0]);
			online_users[split_vector[0] ]=make_pair(split_vector[1],split_vector[2]);
		}
		infile.close();
	}
	else{
		infile.close();
		ofstream outfile(".online.txt",ios::out);
		outfile.close();
	}

	infile.open(".group.txt",ios::in);
	if(infile){
		while(getline(infile,line)){
			vector<string> split_vector=split(line,' ');
			groups[split_vector[0] ]=split_vector[1];
		}
		infile.close();
	}
	else{
		infile.close();
		ofstream outfile(".group.txt",ios::out);
		outfile.close();
	}
}

bool UserLogin(int new_cli,string command1,string command2,string command3,struct Message* message){

	FetchDetails();

	if(credentials.find(command1)==credentials.end()){
		return false;
	}
	else{
		if(command2==credentials[command1]){

			if(online.find(command1)==online.end()){
				online.insert(command1);

				ofstream outfile(".online.txt",ios::out|ios::app);
				outfile<<command1<<" ";
				outfile<<message->client_ip<<" ";
				outfile<<command3;
				outfile<<endl;
			}
			return true;

		}
		else
			return false;
	}
}

bool UserCreate(int new_cli,string command1,string command2,string command3,struct Message* message){

	FetchDetails();

	if(credentials.find(command1)==credentials.end()){
		// online.insert(command1,make_pair(to_string()));
		online.insert(command1);
		ofstream outfile(".online.txt",ios::out|ios::app);
		outfile<<command1<<" ";
		outfile<<message->client_ip<<" ";
		outfile<<command3;
		outfile<<endl;

		outfile.open(".credential.txt",ios::app);
		outfile<<command1;
		outfile<<" ";
		outfile<<command2;
		outfile<<endl;
		outfile.close();

		FetchDetails();

		return true;
	}
	else{
		return false;
	}
}

bool GroupCreate(int new_cli,string command1,string command2,struct Message* message){

	FetchDetails();

	if(groups.find(command1)==groups.end()){
		// online.insert(command1,make_pair(to_string()));

		ofstream outfile(".group.txt",ios::out|ios::app);
		outfile<<command1;
		outfile<<" ";
		outfile<<command2;
		outfile<<endl;
		outfile.close();

		FetchDetails();

		return true;
	}
	else{
		return false;
	}
}

string GroupList(int new_cli,struct Message* message){

	FetchDetails();
	string data="";

	for(auto i:groups){
		data=data+i.first+'\n';
		// cout<<i.first<<" "<<i.second<<endl;
	}

	return data;
}

bool Logout(string command1){

	FetchDetails();

	if(online.find(command1)!=online.end()){
		// online.insert(command1,make_pair(to_string()));

		DeleteLine(".online.txt",command1,0);

		FetchDetails();

		return true;
	}
	else{
		return false;
	}
}

void *TrackerKernel(void *pointer){
	int data_len;
	char data[MAX_SIZE+1];
	vector<string> command_split(10);
	struct Message* message=(struct Message *)pointer;
	// cout<<"yes"<<endl;
	// cout<<message->new_cli<<endl;

	data_len=recv(message->new_cli,data,MAX_SIZE,0);
	data[data_len]='\0';
	cout<<data<<endl;
	// cout<<data_len<<endl;

	string command(data);
	stringstream command_object(command);
	command_object>>command_split[0];

	if(command_split[0]=="login"){
		command_object>>command_split[1];
		command_object>>command_split[2];
		command_object>>command_split[3];

		// cout<<"server "<<command_split[1]<<" "<<command_split[2]<<endl;
		if(UserLogin(message->new_cli,command_split[1],command_split[2],command_split[3],message)){
			send(message->new_cli,"1",1,0);
		}
		else
			send(message->new_cli,"0",1,0);
	}
	else if(command_split[0]=="create_user"){
		command_object>>command_split[1];
		command_object>>command_split[2];
		command_object>>command_split[3];

		// cout<<"server "<<command_split[1]<<" "<<command_split[2]<<endl;
		if(UserCreate(message->new_cli,command_split[1],command_split[2],command_split[3],message)){
			send(message->new_cli,"1",1,0);
		}
		else
			send(message->new_cli,"0",1,0);
	}
	else if(command_split[0]=="create_group"){
		command_object>>command_split[1];
		command_object>>command_split[2];

		// cout<<"server "<<command_split[1]<<" "<<command_split[2]<<endl;
		if(GroupCreate(message->new_cli,command_split[1],command_split[2],message)){
			send(message->new_cli,"1",1,0);
		}
		else
			send(message->new_cli,"0",1,0);
	}
	else if(command_split[0]=="list_groups"){
		string data=GroupList(message->new_cli,message);
		send(message->new_cli,data.c_str(),data.size(),0);
	}
	else if(command_split[0]=="logout"){
		command_object>>command_split[1];

		// cout<<"server "<<command_split[1]<<" "<<command_split[2]<<endl;
		if(Logout(command_split[1])){
			send(message->new_cli,"1",1,0);
		}
		else
			send(message->new_cli,"0",1,0);
	}
	else{}

	return NULL;
}

void *TrackerServer(void *pointer){
	int mysequence_i=*((int*) pointer);
	struct sockaddr_in server,client;
	int sock,new_cli;
	pthread_t tid;

	int sockaddr_len=sizeof(struct sockaddr_in);
	socklen_t len=sizeof(struct sockaddr_in);
	string my_port=tracker_info[mysequence_i].second;
	
	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("server socket: ");
		exit(-1);
	}

	server.sin_family=AF_INET;
	server.sin_port=htons(atoi((tracker_info[mysequence_i].second).c_str()));
	server.sin_addr.s_addr=INADDR_ANY;
	bzero(&server.sin_zero,8);

	if((bind(sock,(struct sockaddr*)&server,sockaddr_len))==-1){
		perror("Bind");
		exit(-1);
	}

	if(listen(sock,5)==-1){
		perror("Listen");
		exit(-1);
	}

	cout<<my_port<<endl;
	while(true){
		if((new_cli=accept(sock,(struct sockaddr*)&client,&len))==-1){
			perror("accept");
			exit(-1);
		}

		Message* message=new Message;
		message->new_cli=new_cli;
		message->client=client;
		// string temp1(tontohs(client.sin_port));
		message->client_port=to_string(ntohs(client.sin_port));
		string temp(inet_ntoa(client.sin_addr));
		message->client_ip=temp;

		auto pid=pthread_create(&tid,NULL,TrackerKernel,(void *)message);
		if(pid!=0){
			perror("thread failed");
			exit(-1);
		}
	}
}

void print_all(){
// 	unordered_map<string,string> credentials;
// unordered_map<string,pair<string,string> >online_users;
// set<string> online;
// unordered_map<string,string> groups;
// vector<pair<string,string>> tracker_info;

	cout<<"##### credentials"<<endl;
	for(auto i:credentials){
		cout<<i.first<<" "<<i.second<<endl;
	}
	cout<<"##### online_users"<<endl;
	for(auto i:online_users){
		cout<<i.first<<" "<<i.second.first<<" "<<i.second.second<<endl;
	}
	cout<<"##### groups"<<endl;
	for(auto i:groups){
		cout<<i.first<<" "<<i.second<<endl;
	}
	cout<<"##### online"<<endl;
	for(auto i:online){
		cout<<i<<endl;
	}
	cout<<"##### tracker_info"<<endl;
	for(auto i:tracker_info){
		cout<<i.first<<" "<<i.second<<endl;
	}
}

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
		if(command=="print"){
			print_all();
		}
	}

	pthread_cancel(tid);
	return 0;
}