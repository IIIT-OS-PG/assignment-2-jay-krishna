#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <set>

using namespace std;

#define MAX_SIZE 1024

#ifndef __peercredentials_H_INCLUDED__
#define __peercredentials_H_INCLUDED__

bool UserLogin(string username,string password);

#endif