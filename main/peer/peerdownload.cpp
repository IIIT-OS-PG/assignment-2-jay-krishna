#include "peerdownload.h"

struct FILEDATA{
	string name;
	string path;
	string hash;
	string size;
	vector<string> seeders;
	unordered_map<string,pair<string,string> > details;
};

vector<string> split2(string data,char delim){
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

void GetChunks(struct FILEDATA* filemeta){
	for(auto i:filemeta->details){
		struct sockaddr_in remote_server;
		int sock;
		char output[MAX_SIZE];

		if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
			perror("socket");
			exit(-1);
		}

		remote_server.sin_family=AF_INET;
		remote_server.sin_port=htons(atoi(i.second.second.c_str()));
		remote_server.sin_addr.s_addr=inet_addr(i.second.first.c_str());
		bzero(&remote_server.sin_zero,8);

		if((connect(sock,(struct sockaddr*)&remote_server,sizeof(struct sockaddr_in)))==-1){
			perror("connect");
			exit(-1);
		}

		string data="Hello "+i.first;
		send(sock,data.c_str(),data.size(),0);
		int len=recv(sock,output,MAX_SIZE,0);
		output[len]='\0';
		cout<<output<<endl;
	}
}

void DownloadFile(string groupname, string filename, string filepath,string username,string myip,string si,int sp){

	struct FILEDATA* filemeta=new FILEDATA;
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

	string data="give_sha "+groupname+" "+filename+" "+username;
	send(sock,data.c_str(),data.size(),0);
	int len=recv(sock,output,MAX_SIZE,0);
	output[len]='\0';
	string filehash(output);

	if(strlen(output)==0){
		cout<<"Invalid request"<<endl;
		return;
	}
	else
		cout<<output<<endl;

	close(sock);

	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(-1);
	}

	if((connect(sock,(struct sockaddr*)&remote_server,sizeof(struct sockaddr_in)))==-1){
		perror("connect");
		exit(-1);
	}

	data="give_seeders "+groupname+" "+filename+" "+username;

	send(sock,data.c_str(),data.size(),0);
	len=recv(sock,output,MAX_SIZE,0);
	output[len]='\0';

	close(sock);

	if(strlen(output)==0){
		cout<<"No seeders available"<<endl;
		return;
	}
	else{
		string output_string(output);
		auto split_vector=split2(output_string,' ');
		// for(auto i:split_vector)
		// 	cout<<i<<endl;
		string size=split_vector[split_vector.size()-1];
		split_vector.erase(split_vector.end()-1);
		// cout<<size<<endl;

		unordered_map<string,pair<string,string> > details;

		for(auto i:split_vector){
			cout<<i<<endl;

			if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
				perror("socket");
				exit(-1);
			}

			if((connect(sock,(struct sockaddr*)&remote_server,sizeof(struct sockaddr_in)))==-1){
				perror("connect");
				exit(-1);
			}

			data="give_details "+i;

			send(sock,data.c_str(),data.size(),0);
			len=recv(sock,output,MAX_SIZE,0);
			output[len]='\0';

			close(sock);

			string cred(output);
			auto split_vector2=split2(cred,' ');

			details[i]=make_pair(split_vector2[0],split_vector2[1]);
		}

		for(auto j:details)
			cout<<j.first<<" "<<j.second.first<<" "<<j.second.second<<endl;

		filemeta->name=filename;
		filemeta->path=filepath;
		filemeta->hash=filehash;
		filemeta->size=size;
		filemeta->seeders=split_vector;
		filemeta->details=details;
	}

	GetChunks(filemeta);

	// close(sock);
	return;
}