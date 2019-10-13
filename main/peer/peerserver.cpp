#include "peerserver.h"

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

string Respond(){
	string data="Hey";

	return data;
}

void* ClientServerKernel(void* pointer){
	int data_len;
	char data[MAX_SIZE+1];
	vector<string> command_split(10);
	struct Message* message=(struct Message *)pointer;

	data_len=recv(message->new_cli,data,MAX_SIZE,0);
	data[data_len]='\0';
	cout<<data<<endl;

	string command(data);
	stringstream command_object(command);
	command_object>>command_split[0];

	if(command_split[0]=="join_group"){
		command_object>>command_split[1];
		command_object>>command_split[2];

		cout<<command_split[1]<<endl;
		cout<<command_split[2]<<endl;
	}
	else if(command_split[0]=="Hello"){
		command_object>>command_split[1];
		// command_object>>command_split[2];

		cout<<command_split[1]<<endl;
		string data=Respond();

		send(message->new_cli,data.c_str(),data.size(),0);
	}
	else{}

	close(message->new_cli);
	return NULL;
}


void* ClientServer(void* pointer){
	int my_port_i=*((int*)pointer);
	cout<<"here"<<endl;
	cout<<my_port_i<<endl;
	int sock;
	int new_cli;
	pthread_t tid;


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

		Message* message=new Message;
		message->new_cli=new_cli;
		message->client=client;
		message->client_port=to_string(ntohs(client.sin_port));
		string temp(inet_ntoa(client.sin_addr));
		message->client_ip=temp;

		auto pid=pthread_create(&tid,NULL,ClientServerKernel,(void *)message);
		if(pid!=0){
			perror("thread failed");
			exit(-1);
		}
	}
}