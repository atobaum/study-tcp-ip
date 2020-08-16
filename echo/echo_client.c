/*
 * echo_client
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>

#define BUFSIZE 1024
void error_handling(char *msg);
int main(int argc, char **argv){
	int sock;
	char message[BUFSIZE];
	int str_len;

	struct sockaddr_in serv_addr;

	if(argc != 3){
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(0);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;

	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");

	while(1){
		fputs("전송할 메시지 입력 (q to quit) : ", stdout);
		fgets(message, BUFSIZE, stdin);

		if(!strcmp(message, "q\n")) break;
		write(sock, message, strlen(message));

		str_len = read(sock, message, BUFSIZE -1);
		message[str_len] = 0;
		printf("From server: %s\n", message);
	}

	close(sock);
	return 0;
}

void error_handling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
