#include "peercredentials.h"

bool UserLogin(string username,string password){
	struct sockaddr_in remote_server;
	int sock;
	char output[MAX_SIZE];

	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(-1);
	}

	remote_server.sin_family=AF_INET;
	remote_server.sin_port=htons(10000);
	remote_server.sin_addr.s_addr=inet_addr("127.0.0.1");
	bzero(&remote_server.sin_zero,8);

	if((connect(sock,(struct sockaddr*)&remote_server,sizeof(struct sockaddr_in)))==-1){
		perror("connect");
		exit(-1);
	}

	string data="login "+username+" "+password;
	send(sock,data.c_str(),data.size(),0);
	int len=recv(sock,output,MAX_SIZE,0);
	output[len]='\0';
	cout<<output<<endl;

	if(output[0]=='0')
		return false;
	else
		return true;

	return true;
}