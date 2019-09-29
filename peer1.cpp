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
	string server_ip(argv[2]);
	string server_port(argv[3]);
	string my_port(argv[1]);

	auto pid=fork();
	if(pid<0){
		perror("Fork Error");
		exit -1;
	}
	else if(pid==0){

	}
	else{
		while(true){
			
		}
	}
	return 0;
}