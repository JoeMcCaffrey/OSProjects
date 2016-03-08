/*

Joseph McCaffrey
Jim Calabro

Date: 
10/30/2015

Class: 
CS 377 Lab 2

Description: 
This program solves the producer consumer problem with a mutex and two counting
semaphores.

There are three command line inputs:

1. The number of consumer threads
2. The max sleep time for the consumer threads
3. The max sleep time for the producer threads

The producer will calculate a random sleep time for the enqueueing job
and then calculate a sleep time for its self.

Consumer will dequeue a job if availible and run
Producer will enqueue a job if the queue is not full


Compilation instructions:
Use the Makefile provided to compile, 
otherwise gcc -o produceConsume produceConsume.c -pthread

Running instructions:

./produceConsume <Number of threads> <max consumer wait time> <max producer wait time>		
Example:
./produceConsume 3 3 3



*/



#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


struct JobData {

	int data;
	int job_id;
};

struct Node {

	struct JobData Job_Data; // this will store the seconds for the consume        
	struct Node * next; // points to the next node
};

struct Node * rear = NULL;
struct Node * front = NULL;
int size_actual = 0;
int queue_size;
// mutex and semaphore definitions
pthread_mutex_t mutex;
sem_t empty;
sem_t full;
int produce_sleep_max;
int consume_sleep_max;
time_t seconds;

// Linked list queue
void Enqueue(int x, int id_num){
        printf("Enqueueing-------------------\n");
	// ask for some memory 
	int i;
 	
	struct Node* temp =  (struct Node*) malloc(sizeof(struct Node));
	struct JobData * job_data = (struct JobData*) malloc(sizeof(struct JobData));
	// this will hold the time for sleep and job id number
	job_data->data = x;
	job_data->job_id = id_num; 

	temp->Job_Data = *job_data;
	
	temp->next = NULL;
	if(front ==NULL && rear == NULL){
		front = rear = temp;
		return;
	}
	rear->next =temp;
	rear = temp; 	

}

struct JobData Dequeue(){

	printf("Dequeueing-------------------\n");
	struct Node* temp = front;

 	struct JobData want = temp->Job_Data;
	
	if(front == NULL){
		printf("queue is empty");
	}
	if(front == rear){
		front = rear = NULL;
	}
	else{
		front = front->next;
	}
	
	free(temp);
	return want;
}

void * Consumer(){
	
	
	seconds= time(NULL);

	printf("Hello from consumer #%u at time %ld\n", (unsigned int)pthread_self(), seconds);
	while(1){

	// sem wait if the queue is empty
		sem_wait(&full);
	// grab the queue lock so we can dequeue
		pthread_mutex_lock(&mutex);

 		struct JobData info = Dequeue();
		int time2 = info.data;

		int job = info.job_id;
	// restore the queue lock because we are done with queue operations
        	pthread_mutex_unlock(&mutex);
	// get the thread id and Unix time
		unsigned int tid;
		tid = (unsigned int) pthread_self();
		seconds = time(NULL);
		printf("thread #%u is going to sleep for %d with job number: %d at time %ld\n",tid, time2,job, seconds);
		sleep(time2);
		seconds= time(NULL);
		printf("thread #%u is done with job number %d at time %ld\n",tid,job,seconds);
	// declare that we are an availible thread
		sem_post(&empty);
		
	}
	
}

void * Producer(){
	
	
	seconds= time(NULL);

	printf("Hello from producer #%u at time %ld\n", (unsigned int) pthread_self(), seconds);
	int id_num =0;
	while(1){
	// sem wait if the queue is full
		sem_wait(&empty);
	// lock the queue because we are going to use it
		pthread_mutex_lock(&mutex);

	// calculate wait time for the thread
		int thread_time = (rand()%(consume_sleep_max+1 -1)) + 1;
		seconds= time(NULL);
		printf("Producer enqueing time: %d and id %d at time %ld\n",thread_time,id_num, seconds);	
	// Enqueue the sleep time and the id number
		Enqueue(thread_time, id_num);
		
	//unlock the buffer because we are done with it
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
		id_num ++;
	// calculate the producer sleep time
  		int producer_time = (rand()%(produce_sleep_max+1 -1)) +1;
		seconds = time(NULL);
		printf("Producer sleeping for %d at time %ld\n",producer_time,seconds);
		sleep(producer_time);		
	}

}		





int main(int argc, char * argv[]){

	pthread_t pro,con;

	if(argc < 3){
		printf("Usage: <Number of threads (n)>, <Max wait time per request(M)>,<Producer sleep factor>\n"); 
		return(1);		
	}

	// parse the command line arguments into ints
	queue_size  = atoi(argv[1]);
 	consume_sleep_max = atoi(argv[2]);
	produce_sleep_max = atoi(argv[3]);   

	//init some sync variables
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, queue_size);
	sem_init(&full , 0, 0);

	//create some Consumer threads before consumer
	int i; 
	for(i=0 ; i< queue_size; i++){
		pthread_create(&con, NULL, Consumer,NULL);
	}
	// wait for consumer threads to come up
	sleep(2);
	// create producer thread
	pthread_create(&pro, NULL, Producer,NULL);
	// main wait
	while(1) { } 	
	return 0;
}
