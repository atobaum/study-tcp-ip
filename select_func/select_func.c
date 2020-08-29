#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/time.h>

#define BUFZISE 30

int main(){
	fd_set reads, temps;
	char msg[BUFZISE];
	int str_len;
	int result;
	struct timeval timeout;

	FD_ZERO(&reads);
	FD_SET(0, &reads);

	while(1){
		temps = reads;
		timeout.tv_sec = 5;
		//microsec
		timeout.tv_usec = 0;

		// number(max fd + 1), readfds, writefds, exceptfds 검사 항목들
		// returns number of changed fd
		result = select(1, &temps, 0, 0, &timeout);
		if(result == -1){
			puts("error: select()");
			exit(1);
		}else if(result == 0){
			puts("timeout");
		} else{
			if(FD_ISSET(0, &reads)){
				str_len = read(0, msg, BUFZISE);
				msg[str_len] = 0;
				printf("input: %s", msg);
			}
		}
	}
}
