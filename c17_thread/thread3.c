#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

void *thread_summation(void *args);

int sum = 0;
int sum1[] = {1, 5};
int sum2[] = {6, 10};

int main(){
	pthread_t id_t1, id_t2;
	void  *t_return;

	pthread_create(&id_t1, NULL, thread_summation, (void *)sum1);
	pthread_create(&id_t2, NULL, thread_summation, (void *)sum2);

	pthread_join(id_t1, &t_return);
	pthread_join(id_t2, &t_return);

	printf("Main함수 종료: %d", sum);
	return 0;
}

void *thread_summation(void *args){
	int start = ((int *)args)[0];
	int end= ((int *)args)[1];

	for(; start <= end; ++start)
		sum += start;
}
