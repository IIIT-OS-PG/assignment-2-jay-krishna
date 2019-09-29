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
#include <sstream>
#include <fstream>

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

	string temp(data);
	stringstream ss(temp);
	string s;
	ss >> s;
	ss >> s;
	s="copy";

	int out;
	int x=1;
	while(x){

		while((len=recv(sock,output,max_size,0))>0){
			// cout<<len<<endl;
			if(count==0){
				string size(output);
				len=atoi(size.c_str());
				ofstream ofs;
				ofs.open(s.c_str(), std::ios::binary | std::ios::out);
				// ofs.seekp(2*len-1);
				for(auto i=0;i<len;++i)
					ofs.write("a",1);
				ofs.close();
				out=open(s.c_str(), O_WRONLY, 0666);
				// lseek(out,0,SEEK_CUR);
				++count;
				cout<<len;
				continue;
			}
			
			cout<<len<<endl;
			write(out,output,len);

		}
		// lseek(out,25,SEEK_CUR);
		// write(out,"Hello",6);
		x=0;
	}
	close(sock);
	return 0;
}