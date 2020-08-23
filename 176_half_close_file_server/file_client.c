#include<stdio.h>
#include<stdlib.h> // atoi exit
#include<string.h>
#include<unistd.h> // read write sleep
#include<fcntl.h>
#include<arpa/inet.h> // htonl inet_addr
#include<sys/types.h>
#include<sys/socket.h>

#define BUFSIZE 30
void error_handling(char *msg);

int main(int argc, char **argv){
	int fd;
	int sd;

	char buf[BUFSIZE];
	int len;
	struct sockaddr_in serv_addr;

	if(argc != 3){
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	fd = open("receive.dat", O_WRONLY|O_CREAT|O_TRUNC);
	if(fd == -1)
		error_handling("file open error");

	sd = socket(PF_INET, SOCK_STREAM, 0);
	if(sd == -1)
		error_handling("socket error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect error");

	while((len = read(sd, buf, BUFSIZE)) != 0){
		write(fd, buf, len);
	}

	write(sd, "Thank tou\n", 10);

	close(fd);
	close(sd);
	return 0;
}

void error_handling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
