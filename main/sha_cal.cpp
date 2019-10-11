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


void getFileHash2(string filename){

   int max_limit=1024;
   unsigned char result[2*SHA_DIGEST_LENGTH];
   unsigned char hash[SHA_DIGEST_LENGTH];
   int i;
   FILE *f = fopen(filename.c_str(),"rb");
   SHA_CTX mdContent;
   unsigned char data[1024+1];
   int bytes;
   // int bytes=fread(data, 1, 1024, f);
   // data[bytes]='\0';
   int count=0;
   while(bytes=fread(data, 1, 1024, f)){
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
   getFileHash2("data.txt");
   return 0;
}