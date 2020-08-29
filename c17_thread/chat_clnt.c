#include<stdio.h>
#include<stdlib.h> // atoi exit
#include<string.h>
#include<unistd.h> // read write sleep
#include<arpa/inet.h> // htonl inet_addr
#include<sys/types.h>
#include<sys/socket.h>
#include <pthread.h>

#define BUFSIZE 100
#define NAMESIZE 20

char name[NAMESIZE] = "[DEFAULT]";
char msg[BUFSIZE];

void *send_msg(void *arg);
void *recv_msg(void *arg);
void error_handling(char *msg);


int main(int argc, char **argv){
	int sock;
	pthread_t snd_thread, rcv_thread;
	void *result;

	int len;
	struct sockaddr_in serv_addr;

	if(argc != 4){
		printf("Usage: %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	}

	sprintf(name, "[%s]", argv[3]);

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect error");

	pthread_create(&snd_thread, NULL, send_msg, (void *)sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void *)sock);

	pthread_join(snd_thread, &result);
	pthread_join(rcv_thread, &result);

	close(sock);
	return 0;
}

void *send_msg(void *arg){
	int sock= (int)arg;
	char name_msg[NAMESIZE+BUFSIZE];
	while(1){
		fgets(msg, BUFSIZE, stdin);
		if(!strcmp(msg, "q\n")){
			close(sock);
			exit(0);
		}
		sprintf(name_msg, "%s %s", name, msg);
		write(sock, name_msg, strlen(name_msg));
	}
}

void *recv_msg(void *arg){
	int sock= (int)arg;
	char name_msg[NAMESIZE+BUFSIZE];
	int str_len;

	while(1){
		str_len = read(sock, name_msg, NAMESIZE+BUFSIZE-1);
		if(str_len == -1) return 1;
		name_msg[str_len] = 0;
		fputs(name_msg, stdout);
	}
}

void error_handling(char *msg){
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
