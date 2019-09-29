#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

#define max_size 1024

int main(int argc, char** argv){
	struct sockaddr_in remote_server;
	int sock;
	// size_t ret_out;
	// char input[max_size];
	char output[max_size+1];
	int len,count=0;

	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(-1);
	}

	remote_server.sin_family=AF_INET;
	remote_server.sin_port=htons(atoi(argv[2]));
	remote_server.sin_addr.s_addr=inet_addr(argv[1]);
	bzero(&remote_server.sin_zero,8);

	if((connect(sock,(struct sockaddr*)&remote_server,sizeof(struct sockaddr_in)))==-1){
		perror("connect");
		exit(-1);
	}

	string data;
	getline(cin,data);
	send(sock,data.c_str(),data.size(),0);

	int out = open("client_copy.jpg", O_WRONLY | O_CREAT | O_APPEND, 0666);
	// FILE *fs;
	// FILE *fs=fopen("client_copy.txt","ab");
	int x=1;
	while(x){
		// string data;
		// getline(cin,data);
		// send(sock,data.c_str(),data.size(),0);

		// len=recv(sock,output,max_size,0);
		// cout<<len<<endl;
		// if(strlen(output)>0){
		// 	++count;
		// 	// cout<<count<<endl;
		// 	output[len]='\0';
		// 	// cout<<output<<endl;

		// 	write(out,output,len);
		// }
		// cout<<"yes"<<endl;

		while((len=recv(sock,output,max_size,0))>0){
			// output[len]='\0';
			cout<<len<<endl;
			// fwrite(output,1,len,fs);

			// if(len<max_size){
			// 	x=0;
			// 	free(fs);
			// }

			write(out,output,len);

		}
		x=0;
	}
	close(sock);
	return 0;
}