#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include<iostream>
#include <unistd.h>
#include<vector>
#include<cmath>
#include<fcntl.h>
#include<sys/wait.h> 
#include <semaphore.h>
// #include<thread>
#include<map>
using namespace std;

// #define numberofthreads 5

#include <openssl/sha.h>

void getFileHash(){

   unsigned char result[2*SHA_DIGEST_LENGTH];
   unsigned char hash[SHA_DIGEST_LENGTH];
   int i;
   FILE *f = fopen("/home/singular/Downloads/final_5d66d1dd7c3b280014ca9936_688500.mp4","rb");
   SHA_CTX mdContent;
   int bytes;
   unsigned char data[14];

   SHA1_Init(&mdContent);
   // while((bytes = fread(data, 1, 1024, f)) != 0){
   //     SHA1_Update(&mdContent, data, bytes);
   // }
   bytes = fread(data, 1, 14, f);
   SHA1_Update(&mdContent, data, bytes);
   SHA1_Final(hash,&mdContent);
   for(i=0; i < SHA_DIGEST_LENGTH;i++){
       sprintf((char *)&(result[i*2]), "%02x",hash[i]);
   }
   printf("%s\n",result);
   fclose(f);
}


void getsha(char *res){

		   FILE *f = fopen("/home/singular/Downloads/final_5d66d1dd7c3b280014ca9936_688500.mp4","rb");
		   char buffer[14];
		   fread(buffer,1,14,f);

		unsigned char SHA_Buffer[SHA_DIGEST_LENGTH];
		char buf[SHA_DIGEST_LENGTH*2];
		int i;
		
		SHA1((unsigned char *)buffer, 14, SHA_Buffer);
		
		for (i=0; i < SHA_DIGEST_LENGTH; i++) {
			sprintf((char*)&(buf[i*2]), "%02x", SHA_Buffer[i]);
		}
		for(int i=0;i<SHA_DIGEST_LENGTH*2;i++){
				res[i]=buf[i];
			}
	// res[i]='\0';
}

void getFileHash2(){

   int max_limit=1024;
   unsigned char result[2*SHA_DIGEST_LENGTH];
   unsigned char hash[SHA_DIGEST_LENGTH];
   int i;
   FILE *f = fopen("/home/singular/Downloads/final_5d66d1dd7c3b280014ca9936_688500.mp4","rb");
   SHA_CTX mdContent;
   unsigned char data[10*1024+1];
   int bytes;
   // int bytes=fread(data, 1, 1024, f);
   // data[bytes]='\0';
   int count=0;
   while(bytes=fread(data, 1, 10*1024, f)){
      data[bytes]='\0';

      SHA1_Init(&mdContent);
      
      SHA1_Update(&mdContent, data, bytes);
      SHA1_Final(hash,&mdContent);
      for(i=0; i < SHA_DIGEST_LENGTH;i++){
          sprintf((char *)&(result[i*2]), "%02x",hash[i]);
      }
      // cout<<data<<endl;
      printf("%s\n",result);
      cout<<bytes<<endl;
      cout<<count<<endl;
      ++count;
      // cout<<strlen(data);
      // int bytes=fread(data, 1, 1024, f);
      // data[bytes]='\0';
   }
   fclose(f);
}


int main () {
   // pthread_t threads;
   // int j=0;
   // int s = 102400;
   char ch='0';
   char *res = new char[SHA_DIGEST_LENGTH*2];
   // getsha(res);
   // // getFileHash();
   // string s="";
   // for(int i=0;i<20;i++)
   // {
   // 		s= s+res[i];
   // }
   // cout << s;
   // cout << endl;
   getFileHash2();
   return 0;
}