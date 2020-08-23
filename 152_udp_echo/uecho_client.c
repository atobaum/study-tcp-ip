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
	int sock;
	char msg[BUFSIZE];
	int str_len, addr_size;

	struct sockaddr_in serv_addr;
	struct sockaddr_in from_addr;

	if(argc != 3){
		printf("Usave: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		error_handling("error socket()");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_family = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	while(1){
		fputs("정송할 메시지 입력 (q to exit): ", stdout);
		fgets(msg, BUFSIZE, stdin);
		if(!strcmp("q\n", msg)) break;

		sendto(sock, msg, strlen(msg), 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
		addr_size = sizeof(from_addr);
		printf("%d\n", addr_size);
		str_len = recvfrom(sock, msg, BUFSIZE, 0, (struct sockaddr*) &from_addr, (socklen_t*) &addr_size);

		msg[str_len] = 0;
		printf("From server: %s", msg);
	}

	close(sock);
	return 0;
}

void error_handling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
