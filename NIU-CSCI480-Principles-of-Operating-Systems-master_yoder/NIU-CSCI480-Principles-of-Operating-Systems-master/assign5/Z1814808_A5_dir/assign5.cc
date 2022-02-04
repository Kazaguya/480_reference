#include<iostream>
#include<semaphore.h>
#include<unistd.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<iomanip>

using namespace std;

#define P_NUMBER 6 //number of producers
#define C_NUMBER 4 //number of consumers
#define BUFFER_SIZE 12 //maximum size of the buffer
#define N_P_STEPS 4 //number of iterations for each producer thread
#define N_C_STEPS 6 //number of iterations for each consumer thread

void Insert(int threadID);
void Remove(int threadID);
void *  Produce(void * threadID);
void * Consume(void *  threadID);

int widget = 0; //total counter for number of widgets

sem_t notFull; //not full semaphore
sem_t notEmpty; //not emtpy seamphore

pthread_mutex_t mutex; //mutex

int main(int argc, char *argv[])
{
	int rc; //return code from the pthread creats

	sem_init(&notFull, 0, BUFFER_SIZE); //set up semphore for max
	sem_init(&notEmpty, 0, 0); //set up semphore for not empty

	pthread_t Consumer[C_NUMBER];	//make array for consumer threads
	pthread_t Producer[P_NUMBER];	//make array for producer threads

	pthread_mutex_init(&mutex, NULL);	//set up mutex

	cout << "Simulation of Producer and Consumers" <<endl;
	cout << "The semaphores and mutex have been initialized." <<endl;

	for (long i = 0; i < C_NUMBER; i++){	//loop and make all of the cousumer thread
		rc = pthread_create(&Consumer[i], NULL, Consume, (void *) i);	//make the threads
		if(rc){	//if there is an error exit
			cerr << "Error in Consumer create" <<endl;
			exit(-1);
		}
	}

	for (long i = 0; i < P_NUMBER; i++){	//make all the producer threads
		rc = pthread_create(&Producer[i], NULL, Produce, (void *) i);
		if(rc){	//If there is an error exit
			cerr << "Error in producer create" <<endl;
			exit(-1);
		}
	}

	for (int i = 0; i < C_NUMBER; i++){	//at the end join delete the threads
		pthread_join(Consumer[i], NULL);
	}

	for(int i = 0; i < P_NUMBER; i++){	//delete the threads
		pthread_join(Producer[i], NULL);
	}

	cout << "All the producer and consumer threads have been closed." <<endl;
	cout << "The semaphores and mutex have been deleted." << endl;

	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);

	sem_destroy(&notFull);	//delete the not full semphore
	sem_destroy(&notEmpty);	//delete the not emtpy seamphore

	return 0;
}

//Function that inserts one widget into the 
//counter. It locks adds 1 and then unlocks.
void Insert(int threadID){
	int code;

	code = pthread_mutex_lock(&mutex);	//try and lock the mutex

	if(code == 0){	//If there was a problem
		widget++;	//add 1 to the counter
		cerr << "Producer " <<threadID << " inserted one item.  Total is now " << widget <<endl;
	}
	else{
		cerr << "Producer " << threadID << " Error in lock" <<endl;	//quit
		exit(-1);
	}

	code = pthread_mutex_unlock(&mutex);	//Unlock the mutex

	if (code != 0){	//if there is a problem quit
		cerr << "Producer " << threadID << " Error in Unlock" <<endl;
		exit(-1);
	}
}

//Function that takes one widget out of
//the counter. It locks and then removes
//the lock.
void Remove(int threadID) {
	int code;

	code = pthread_mutex_lock(&mutex);	//lock the mutex

	if (code == 0) {	//If there is a problem quit
		widget--;	//remove one widget
		cerr << "Consumer " << threadID << " removed one item.   Total is now " << widget << endl;
	}
	else{
		cerr << "Consumer " << threadID << ": Failure in Remove: lock" << endl;
		exit(-1);
	}

	code = pthread_mutex_unlock(&mutex);	//unlock the mutex

	if (code != 0) {
		cerr << "Consumer : " << threadID << ": Failure in Remove: Unlock" << endl;
		exit(-1);
	}
}

//Function that runs each thread to 
//insert into the widgets buffer.
void * Produce(void * threadID) {
	long temp = (long) threadID;	//convert the threadID to a long

	for (int i = 0; i < N_P_STEPS; i++) {//loop for all the number of iterations for produce
		sem_wait(&notFull);	//enter critical section
		Insert(temp);//try and add one to the coutner
		sem_post(&notEmpty);//single out
		sleep(1);//wait 1 sec
	}
	pthread_exit(NULL);//kill thread after all iterations
}

//Function that runs the threads to 
//delete the widgets from the buffer.
void * Consume(void * threadID) {
	long temp = (long) threadID;	//conver to int

	for (int i = 0; i < N_C_STEPS; i++) {//loop through
		sem_wait(&notEmpty);//single ready
		Remove(temp);	//try and remove 1 element
		sem_post(&notFull);//single out
		sleep(1);//sleep
	}
	pthread_exit(NULL);	//end thread
}
