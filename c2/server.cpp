#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;

#define max_size 1024

int main(int argc, char** argv){
	struct sockaddr_in server;
	struct sockaddr_in client;
	int sock;
	int new_cli;
	int sockaddr_len=sizeof(struct sockaddr_in);
	int data_len;
	char data[max_size];

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

	while(-1){
		socklen_t len=sizeof(struct sockaddr_in);
		if((new_cli=accept(sock,(struct sockaddr*)&client,&len))==-1){
			perror("accept");
			exit(-1);
		}

		cout<<"New client connected from port "<<ntohs(client.sin_port)<<" & IP "<<inet_ntoa(client.sin_addr)<<endl;
		data_len=1;
		while(data_len){
			data_len=recv(new_cli,data,max_size,0);
			if(data_len){
				send(new_cli,data,data_len,0);
				data[data_len]='\0';
				cout<<"Recv: "<<data<<" len: "<<data_len<<endl;
			}
		}

		cout<<"client disconnected"<<endl;
		close(new_cli);
	}
	return 0;
}