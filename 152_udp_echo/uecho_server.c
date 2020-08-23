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
	int serv_sock;
	char msg[BUFSIZE];
	int str_len;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;

	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(serv_sock == -1){
		error_handling("socket error()");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind error()");

	printf("Start to listent port %s", argv[1]);
	while(1){
		clnt_addr_size = sizeof(clnt_addr);
		// int sock, int *buf, int buf_len, unsigned flads, addr, addrlen 
		str_len = recvfrom(serv_sock, msg, BUFSIZE, 0, (struct sockaddr*) &clnt_addr, (socklen_t*) &clnt_addr_size);
		
		// int sock, const void *msg, int len, flas, addr, addrlen
		sendto(serv_sock, msg, str_len, 0, (struct sockaddr*) &clnt_addr, sizeof(clnt_addr));
	}

	close(serv_sock);
	return 0;
}

void error_handling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
