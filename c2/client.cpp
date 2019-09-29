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
	struct sockaddr_in remote_server;
	int sock;
	char input[max_size];
	char output[max_size];
	int len;

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

	while(1){
		cin>>input;
		send(sock,input,strlen(input),0);

		len=recv(sock,output,max_size,0);
		output[len]='\0';
		cout<<output<<endl;
	}
	close(sock);
	return 0;
}