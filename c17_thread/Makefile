CC = gcc

chat_server : chat_server.c
	$(CC) -D_REENTRANT $^ -o $@ -lpthread

chat_clnt: chat_clnt.c
	$(CC) -D_REENTRANT $^ -o $@ -lpthread
