#include<stdio.h>
#include<stdlib.h> // atoi exit
#include<string.h>
#include<unistd.h> // read write sleep
#include<arpa/inet.h> // htonl inet_addr
#include<sys/types.h>
#include<sys/socket.h>

#define BUFSIZE 30
void error_handling(char *msg);

int main(int argc, char **argv){

}

void error_handling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
