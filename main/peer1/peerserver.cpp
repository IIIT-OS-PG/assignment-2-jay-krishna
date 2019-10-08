#include "peerserver.h"


void* ClientServer(void* pointer){
	int my_port_i=*((int*)pointer);
	// string my_port_s(to_string(my_port_i));
	cout<<"here"<<endl;
	cout<<my_port_i<<endl;
	int sock;
	int new_cli;
	int data_len;
	char data[MAX_SIZE];


	struct sockaddr_in server;
	struct sockaddr_in client;
	int sockaddr_len=sizeof(struct sockaddr_in);

	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1){
		perror("server socket: ");
		exit(-1);
	}

	server.sin_family=AF_INET;
	server.sin_port=htons(my_port_i);
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

	while(1){
		socklen_t len=sizeof(struct sockaddr_in);
		if((new_cli=accept(sock,(struct sockaddr*)&client,&len))==-1){
			perror("accept");
			exit(-1);
		}

		// cout<<"New client connected from port "<<ntohs(client.sin_port)<<" & IP "<<inet_ntoa(client.sin_addr)<<endl;
		data_len=1;
		while(data_len){
			data_len=recv(new_cli,data,MAX_SIZE,0);
			if(data_len){
				// send(new_cli,data,data_len,0);
				data[data_len]='\0';
				cout<<"Recv: "<<data<<" len: "<<data_len<<endl;
			}
		}

		// cout<<"client disconnected"<<endl;
		close(new_cli);
	}
}