# Thread
the relation between producer and consumer
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define N 10
sem_t empty,full,mutex;
int buffer[N],in = 0,out = 0;

int new_data;
int out_data;

void *producer(void *arg);
void *consumer(void *arg);
int producer_id;
int consumer_id;
char message[20]="Hello world";

void main()
{
	system("clear");
	int i;
	
	pthread_t tid_producer[5],tid_consumer[5];
	srand((unsigned)time(NULL));
	printf("start...\n");
	sem_init(&empty,0,N);
	sem_init(&full,0,0);
	sem_init(&mutex,0,1);
	for(i=0;i<5;i++)
	{
		pthread_create(&tid_producer[i],NULL,producer,(void *)message);
		pthread_create(&tid_consumer[i],NULL,consumer,NULL);
	}
	for(unsigned int j=0;j<65535;j++);

	printf("end...\n");
}

void *producer(void *arg)
{
	int i;
	while(1){
		new_data = rand()%1000;
		sem_wait(&empty);
		sem_wait(&mutex);
		buffer[in] = new_data;
		printf("Producer id : %lu   ",pthread_self()%10);
		printf("put buffer[%d] = %d\n",in,buffer[in]);
		in = (in+1)%N;
		sem_post(&mutex);
		sem_post(&full);
	}
	
	sleep(1);
}

void *consumer(void *arg)
{
	while(1){
		
		sem_wait(&full);
		sem_wait(&mutex);
		out_data = buffer[out];
		printf("consumer id : %lu   ",pthread_self()%10);
		printf("got buffer[%d] = %d\n",out,out_data);
		buffer[out] = 0;
		out = (out+1)%N;
		sem_post(&mutex);
		sem_post(&empty);
	}
			
	sleep(1);
}
