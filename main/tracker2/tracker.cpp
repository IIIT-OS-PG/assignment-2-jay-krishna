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
#include <pthread.h>

#include "trackersync.h"

#define MAX_SIZE 1024

using namespace std;

unordered_map<string,string> credentials;
unordered_map<string,pair<string,string> >online_users;
set<string> online;
set<string>group_members;
unordered_map<string,string> groups;
vector<pair<string,string>> tracker_info;
set<pair<string,string> > pending_group_requests;

struct Message{
	int new_cli;
	struct sockaddr_in client;
	string client_ip;
	string client_port;
};

int mysequence_i;

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
	outfile.close();
}

void DeleteLine(string filename,string data){
	string line;
	ifstream infile(filename,ios::in);
	// cout<<"Whole line "<<data<<endl;
	ofstream outfile(".temp.txt",ios::out|ios::app);

	while(getline(infile,line)){
			if(line==data)
				continue;
			else{
				cout<<line<<endl;
				outfile<<line<<endl;
			}
	}

	remove(filename.c_str());
	rename(".temp.txt",filename.c_str());
	infile.close();
}

bool CheckGroupFiles(string filehash,string filename,string username){
	ifstream infile(filename,ios::in);
	string line;

	while(getline(infile,line)){
		auto file_vector=split(line,' ');
		if(file_vector[0]==filehash && file_vector[2]==username){
			cout<<filehash<<" "<<username<<endl;
			cout<<file_vector[0]<<" "<<file_vector[2]<<endl;
			return true;
		}
	}
	return false;
}

string GetGroupFileName(string filehash,string filename){
	ifstream infile(filename,ios::in);
	string line;
	string name="";

	while(getline(infile,line)){
		auto file_vector=split(line,' ');
		if(file_vector[0]==filehash){
			name=file_vector[3];
			break;
		}
	}

	return name;
}

bool CheckIntegrity(string filehash,string filename,string upload_file){
	ifstream infile(filename,ios::in);
	string line;

	while(getline(infile,line)){
		auto file_vector=split(line,' ');
		cout<<line<<endl;
		if(file_vector[0]!=filehash && file_vector[3]==upload_file){
			return true;
		}
	}

	return false;
}

void FetchDetails(){
	credentials.clear();
	online.clear();
	online_users.clear();
	groups.clear();
	pending_group_requests.clear();
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

	infile.open(".pending.txt",ios::in);
	if(infile){
		while(getline(infile,line)){
			stringstream line_object(line);
			line_object>>line1;
			line_object>>line2;

			pending_group_requests.insert(make_pair(line1,line2));
		}
		infile.close();
	}
	else{
		infile.close();
		ofstream outfile(".pending.txt",ios::out);
		outfile.close();
	}
}

void FetchGroupMembers(string group_name){
	string name=".group_"+group_name+".txt";
	string line;
	group_members.clear();
	ifstream infile(name,ios::in);
	// cout<<name<<endl;
	while(getline(infile,line)){
		// cout<<line<<endl;
		group_members.insert(line);
	}
}

void SendMessage(string ip,string port,string data){
	struct sockaddr_in remote_server;
	int sock;

	cout<<ip<<endl;
	cout<<port<<endl;
	cout<<data<<endl;

	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(-1);
	}

	remote_server.sin_family=AF_INET;
	remote_server.sin_port=htons(atoi(port.c_str()));
	remote_server.sin_addr.s_addr=inet_addr(ip.c_str());
	bzero(&remote_server.sin_zero,8);

	if((connect(sock,(struct sockaddr*)&remote_server,sizeof(struct sockaddr_in)))==-1){
		perror("connect");
		exit(-1);
	}

	send(sock,data.c_str(),data.size(),0);
	close(sock);
}

bool UserLogin(int new_cli,string command1,string command2,string command3,string command4,struct Message* message){

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
				outfile<<command4<<" ";
				outfile<<command3;
				outfile<<endl;

				Sync(".online.txt",mysequence_i,tracker_info);
			}
			return true;

		}
		else
			return false;
	}
}

bool UserCreate(int new_cli,string command1,string command2,string command3,string command4,struct Message* message){

	FetchDetails();

	if(credentials.find(command1)==credentials.end()){
		// online.insert(command1,make_pair(to_string()));
		online.insert(command1);
		ofstream outfile(".online.txt",ios::out|ios::app);
		outfile<<command1<<" ";
		outfile<<command4<<" ";
		outfile<<command3;
		outfile<<endl;

		ofstream outfile2(".credential.txt",ios::out|ios::app);
		// if(outfile2){
		// 	cout<<"open"<<endl;
		// }
		// else
		// 	cout<<"Not open"<<endl;
		outfile2<<command1;
		outfile2<<" ";
		outfile2<<command2;
		outfile2<<endl;
		outfile.close();

		string name=".files_"+command1+".txt";
		ofstream outfile3(name,ios::out);
		outfile3.close();

		Sync(".online.txt",mysequence_i,tracker_info);
		Sync(".credential.txt",mysequence_i,tracker_info);
		Sync(name,mysequence_i,tracker_info);

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

		string name=".group_"+command1+".txt";
		outfile.open(name,ios::out);
		outfile<<command2;
		outfile<<endl;
		outfile.close();

		string name1=".group_"+command1+"_files.txt";
		outfile.open(name1,ios::out);
		outfile.close();

		Sync(".group.txt",mysequence_i,tracker_info);
		Sync(name,mysequence_i,tracker_info);
		Sync(name1,mysequence_i,tracker_info);

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

string GroupRequestList(int new_cli,struct Message* message,string command1,string command2){

	FetchDetails();
	string data="";

	auto iter=groups.find(command1);

	if(iter==groups.end() || command2!=iter->second)
		return data;
	else{
		for(auto i:pending_group_requests){
			// cout<<i.first<<" "<<i.second<<endl;
			if(i.second==command1){
				data=data+i.first+'\n';
			}
		}
	}

	return data;
}

string GroupFileList(int new_cli,struct Message* message,string command1){

	FetchDetails();
	string data="";

	auto iter=groups.find(command1);

	if(iter==groups.end())
		return data;
	else{

		set<string> files;
		string name=".group_"+command1+"_files.txt";
		string line;

		ifstream infile(name,ios::in);

		while(getline(infile,line)){
			auto split_vector=split(line,' ');
			files.insert(split_vector[3]);
		}

		for(auto i:files){
			data=data+i+'\n';
		}
	}

	return data;
}

bool GroupJoin(int new_cli,string command1,string command2,struct Message* message){
	auto iter=groups.find(command1);

	if(iter==groups.end())
		return false;
	// cout<<"Output "<<command1<<" "<<command2<<" "<<iter->first<<" "<<iter->second<<endl;
	FetchGroupMembers(command1);
	if(group_members.find(command2)!=group_members.end()){
		cout<<"Already Memeber"<<endl;
		return true;
	}

	auto iter2=online.find(iter->second);
	if(iter2==online.end()){
		if(pending_group_requests.find(make_pair(command2,command1))==pending_group_requests.end()){
			ofstream outfile(".pending.txt",ios::out|ios::app);
			outfile<<command2<<" "<<command1<<endl;
			outfile.close();
			pending_group_requests.insert(make_pair(command2,command1));

			
		}
	}
	else{
		auto iter3=online_users[*iter2];
		// cout<<iter3.first<<" "<<iter3.second<<endl;

		ofstream outfile(".pending.txt",ios::out|ios::app);
		outfile<<command2<<" "<<command1<<endl;
		outfile.close();
		pending_group_requests.insert(make_pair(command2,command1));

		// string data="join_group "+command1+" "+command2;
		// cout<<"2nd"<<endl;
		// SendMessage(iter3.first,iter3.second,data);

		// Sync(".pending.txt");
	}

	Sync(".pending.txt",mysequence_i,tracker_info);
	return true;
}

bool GroupAcceptRequest(int new_cli,string command1,string command2,string command3,struct Message* message){
	auto iter=groups.find(command1);
	auto iter1=credentials.find(command2);
	auto iter2=pending_group_requests.find(make_pair(command2,command1));
	// cout<<"group accept"<<endl;

	if(iter==groups.end() || iter->second != command3 || iter1==credentials.end() || iter2==pending_group_requests.end())
		return false;

	FetchGroupMembers(command1);
	if(group_members.find(command2)!=group_members.end()){
		cout<<"Already Memeber"<<endl;
		return true;
	}
	string name=".group_"+command1+".txt";
	string remove_line=command2+" "+command1;
	// cout<<name<<endl;
	ofstream outfile(name,ios::out|ios::app);
	outfile<<command2<<endl;
	outfile.close();
	DeleteLine(".pending.txt",remove_line);

	Sync(".pending.txt",mysequence_i,tracker_info);

	return true;
}

bool GroupFileUpload(int new_cli,string command1,string command2,string command3,string command4,string command5,string command6,struct Message* message){
	auto iter=groups.find(command3);

	// cout<<command1<<" "<<command2<<" "<<command3<<" "<<command4<<" "<<command5<<" "<<command6<<endl;


	if(iter==groups.end())
		return false;
	
	FetchGroupMembers(command3);
	if(group_members.find(command4)==group_members.end())
		return false;
	string name=".group_"+command3+"_files.txt";

	if(CheckGroupFiles(command1,name,command4))
		return true;

	string file_name=GetGroupFileName(command1,name);
	if(file_name.size()>0)
		command5=file_name;

	if(CheckIntegrity(command1,name,command5))
		return false;

	ofstream outfile(name,ios::out|ios::app);
	outfile<<command1<<" "<<command2<<" "<<command4<<" "<<command5<<endl;
	
	string name2=".files_"+command4+".txt";
	ofstream outfile2(name2,ios::out|ios::app);
	outfile2<<command3<<" "<<command5<<" "<<command6<<endl;

	Sync(name,mysequence_i,tracker_info);
	Sync(name2,mysequence_i,tracker_info);

	return true;
}

bool GroupStopShare(int new_cli,string command1,string command2,string command3,struct Message* message){
	auto iter=groups.find(command2);
	auto iter1=credentials.find(command3);

	cout<<command1<<" "<<command2<<" "<<command3<<endl;

	if(iter==groups.end() || iter1==credentials.end())
		return false;

	cout<<command1<<" "<<command2<<" "<<command3<<endl;
	FetchGroupMembers(command2);
	if(group_members.find(command3)==group_members.end()){
		return false;
	}
	cout<<command1<<" "<<command2<<" "<<command3<<endl;

	string name=".group_"+command2+"_files.txt";
	string name1=".files_"+command3+".txt";

	ifstream infile(name,ios::in);
	ofstream outfile(".temp.txt",ios::out);
	string line;

	while(getline(infile,line)){
		auto split_vector=split(line,' ');
		if(split_vector[3]==command1 && split_vector[2]==command3)
			continue;
		outfile<<line<<endl;
	}

	remove(name.c_str());
	rename(".temp.txt",name.c_str());
	infile.close();
	outfile.close();

	DeleteLine(name1,command1,1);

	Sync(name,mysequence_i,tracker_info);
	Sync(name1,mysequence_i,tracker_info);

	return true;
}

bool GroupLeave(int new_cli,string command1,string command2,struct Message* message){
	auto iter=groups.find(command1);

	if(iter==groups.end())
		return false;

	FetchGroupMembers(command1);
	if(group_members.find(command2)==group_members.end() || iter->second==command2){
		return false;
	}
	
	string name1=".group_"+command1+".txt";
	string name2=".group_"+command1+"_files.txt";
	string name3=".files_"+command2+".txt";

	DeleteLine(name1,command2,0);
	DeleteLine(name2,command2,2);
	DeleteLine(name3,command1,0);

	Sync(name1,mysequence_i,tracker_info);
	Sync(name2,mysequence_i,tracker_info);
	Sync(name3,mysequence_i,tracker_info);

	FetchDetails();

	return true;
}

bool Logout(string command1){

	FetchDetails();

	if(online.find(command1)!=online.end()){
		// online.insert(command1,make_pair(to_string()));

		DeleteLine(".online.txt",command1,0);
		Sync(".online.txt",mysequence_i,tracker_info);

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
	memset(data,'\0', (MAX_SIZE+1)*sizeof(data[0]));
	vector<string> command_split(10);
	struct Message* message=(struct Message *)pointer;
	// cout<<"yes"<<endl;
	// cout<<message->new_cli<<endl;


	data_len=recv(message->new_cli,data,MAX_SIZE,0);
	data[data_len]='\0';
	// cout<<data<<endl;
	// cout<<data_len<<endl;

	string command(data);
	stringstream command_object(command);
	command_object>>command_split[0];

	if(command_split[0]=="login"){
		command_object>>command_split[1];
		command_object>>command_split[2];
		command_object>>command_split[3];
		command_object>>command_split[4];

		// cout<<"server "<<command_split[1]<<" "<<command_split[2]<<endl;
		if(UserLogin(message->new_cli,command_split[1],command_split[2],command_split[3],command_split[4],message)){
			send(message->new_cli,"1",1,0);
		}
		else
			send(message->new_cli,"0",1,0);
	}
	else if(command_split[0]=="create_user"){
		command_object>>command_split[1];
		command_object>>command_split[2];
		command_object>>command_split[3];
		command_object>>command_split[4];

		// cout<<"server "<<command_split[1]<<" "<<command_split[2]<<endl;
		if(UserCreate(message->new_cli,command_split[1],command_split[2],command_split[3],command_split[4],message)){
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
	else if(command_split[0]=="join_group"){
		command_object>>command_split[1];
		command_object>>command_split[2];

		// cout<<"server "<<command_split[1]<<" "<<command_split[2]<<endl;
		if(GroupJoin(message->new_cli,command_split[1],command_split[2],message)){
			send(message->new_cli,"1",1,0);
		}
		else
			send(message->new_cli,"0",1,0);
	}
	else if(command_split[0]=="list_requests"){
		command_object>>command_split[1];
		command_object>>command_split[2];

		string data=GroupRequestList(message->new_cli,message,command_split[1],command_split[2]);
		send(message->new_cli,data.c_str(),data.size(),0);
	}
	else if(command_split[0]=="accept_request"){
		command_object>>command_split[1];
		command_object>>command_split[2];
		command_object>>command_split[3];

		if(GroupAcceptRequest(message->new_cli,command_split[1],command_split[2],command_split[3],message)){
			send(message->new_cli,"1",1,0);
		}
		else
			send(message->new_cli,"0",1,0);
	}
	else if(command_split[0]=="upload_file"){
		command_object>>command_split[1];
		command_object>>command_split[2];
		command_object>>command_split[3];
		command_object>>command_split[4];
		command_object>>command_split[5];
		command_object>>command_split[6];

		if(GroupFileUpload(message->new_cli,command_split[1],command_split[2],command_split[3],command_split[4],command_split[5],command_split[6],message)){
			send(message->new_cli,"1",1,0);
		}
		else
			send(message->new_cli,"0",1,0);
	}
	else if(command_split[0]=="list_files"){
		command_object>>command_split[1];

		string data=GroupFileList(message->new_cli,message,command_split[1]);
		send(message->new_cli,data.c_str(),data.size(),0);
	}
	else if(command_split[0]=="stop_share"){
		command_object>>command_split[1];
		command_object>>command_split[2];
		command_object>>command_split[3];

		if(GroupStopShare(message->new_cli,command_split[1],command_split[2],command_split[3],message)){
			send(message->new_cli,"1",1,0);
		}
		else
			send(message->new_cli,"0",1,0);
	}
	else if(command_split[0]=="leave_group"){
		command_object>>command_split[1];
		command_object>>command_split[2];

		if(GroupLeave(message->new_cli,command_split[1],command_split[2],message)){
			send(message->new_cli,"1",1,0);
		}
		else
			send(message->new_cli,"0",1,0);
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
	else if(command_split[0]=="Sync"){
		command_object>>command_split[1];

		SyncRecv(message->new_cli,command_split[1]);
	}
	else{}

	close(message->new_cli);
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

	FetchDetails();

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

	cout<<"##### pending"<<endl;
	for(auto i:pending_group_requests){
		cout<<i.first<<" "<<i.second<<endl;
	}
}

int main(int argc, char** argv){

	bool flag=true;
	string command,line,temp_ip,temp_port;
	pthread_t tid;

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