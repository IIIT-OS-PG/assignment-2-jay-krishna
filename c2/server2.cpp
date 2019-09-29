#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sstream>
#include <pthread.h>
#include <fstream>
#include <stdio.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>

using namespace std;

#define max_size 1024

void* socketthread(void *pointer){
	int data_len;
	char data[max_size+1];
	int count=0;
	int new_cli=*((int *)pointer);

	data_len=1;
	data_len=recv(new_cli,data,max_size,0);
	while(data_len){
		// data_len=recv(new_cli,data,max_size,0);
		if(data_len){
			data[data_len]='\0';
			string temp(data);
			stringstream ss(temp);
			string s;
			ss >> s;
			if(s=="sendfile"){
				ss >> s;
				cout<<s<<endl;

				FILE *fs=fopen(s.c_str(),"rb");
				int x=1;
				while(x){
					int nread=fread(data,1,max_size,fs);
					if(nread<max_size){
						data[nread]='\0';
						x=0;	
					}
					if(nread>0){
						// cout<<data<<endl;
						send(new_cli,data,nread,0);
					}
					cout<<nread<<endl;
				}

				
				// int in=open(s.c_str(),O_RDONLY);
				// size_t ret_in;
				// while((ret_in = read (in, &data, 1024)) > 0){
				// 	// if(count==limit){
				// 	// 	close(out);
				// 	// 	++curr;
				// 	// 	destptf=destpt+to_string(curr)+".mp4";
				// 	// 	// destpt[destpt.size()-1]=to_string(curr);
				// 	// 	int out = open(destptf.c_str(), O_WRONLY | O_CREAT, 0600);
				// 	// 	count=0;
				// 	// 	cout<<destptf<<endl;
				// 	// }
				// 	// ret_out=write(out,str,ret_in);
	   //  //         	++count;
				// 	data[ret_in]='\0';
				// 	// cout<<"##################################"<<endl;
				// 	// cout<<data<<endl;
				// 	cout<<count<<endl;
				// 	++count;
				// 	send(new_cli,data,strlen(data),0);
    // 			}
    			data_len=0;
			}
			else{
				send(new_cli,data,data_len,0);
				cout<<"Recv: "<<data<<" len: "<<data_len<<endl;
			}
		}
	}

	cout<<"client disconnected"<<endl;
	cout<<count<<endl;
	close(new_cli);
	pthread_exit(NULL);
}

int main(int argc, char** argv){
	struct sockaddr_in server;
	struct sockaddr_in client;
	int sock;
	int new_cli;
	int sockaddr_len=sizeof(struct sockaddr_in);

	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("server socket: ");
		exit(-1);
	}

	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[1]));
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
	pthread_t tid;
	while(1){
		socklen_t len=sizeof(struct sockaddr_in);
		if((new_cli=accept(sock,(struct sockaddr*)&client,&len))==-1){
			perror("accept");
			exit(-1);
		}

		// cout<<"New client connected from port "<<ntohs(client.sin_port)<<" & IP "<<inet_ntoa(client.sin_addr)<<endl;


		auto pid=pthread_create(&tid,NULL,socketthread,&new_cli);
		if(pid!=0){
			perror("thread failed");
			exit(-1);
		}
		else{
			cout<<"Success"<<endl;
		}
	}
	return 0;
}