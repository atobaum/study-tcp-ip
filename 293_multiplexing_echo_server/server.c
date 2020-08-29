/*
 * hellowerworld_server
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 100
void error_handling(char *msg);

int main(int argc, char **argv){
	int serv_sock;
	struct sockaddr_in serv_addr;

	fd_set reads, temps;
	int fd_max;

	char *buffer[BUFSIZE];
	int str_len;
	struct timeval timeout;

	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		exit(0);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");

	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;

	while(1){
		int fd, str_len;
		int clnt_sock, clnt_len;
		struct sockaddr_in clnt_addr;

		temps = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		if(select(fd_max, &temps, 0, 0, &timeout) == -1)
			error_handling("select() error");
		for (fd = 0; fd < fd_max; ++fd){
			if(FD_ISSET(fd, &temps)){
				if(fd == serv_sock){ // 연결요청
					printf("connect");
					clnt_len = sizeof(clnt_sock);
					clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_len);

					FD_SET(clnt_sock, &reads);
					if(fd_max < clnt_sock)
						fd_max = clnt_sock;
				}else{
					str_len = read(fd, buffer, BUFSIZE);
					if(str_len == 0) {//연결종료	
						FD_CLR(fd, &reads);
						close(fd);
						printf("Close client %d", fd);
					} else{
						write(fd, buffer, str_len);
					}
				}	
			}
		}

		if(clnt_sock == -1)
			error_handling("accept() error");

		write(clnt_sock, buffer, sizeof(buffer));
		close(clnt_sock);
	}
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
