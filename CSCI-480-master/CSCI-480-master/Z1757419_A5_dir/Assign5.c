/*******************************************************************************
   PROGRAM:			CSCI 480 Assignment 5
   PROGRAMMER:	Joseph McDade
   LOGON ID:		z1757419
   DUE DATE:		10/26/2018
	 Note:				Uses hard tabs set to display as 2 spaces

   FUNCTION:		Producer and Consumer problem using multiple producer and
	 							consumer POSIX threads, semaphores, and mutexes.
*******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

// Prototyping statements
void* produce_run(void* tID);
void* consume_run(void* tID);
void insert_widget(int);
void remove_widget(int);

// Set global constants
#define P_NUMBER 6
#define C_NUMBER 4
#define BUFFER_SIZE 12
#define N_P_STEPS 4
#define N_C_STEPS 6

// Create buffer
size_t widgets = 0;
// Create semaphores and mutex
sem_t not_full, not_empty;
pthread_mutex_t counter_lock;

int main() {
	// Create pthreads
	pthread_t	producers[P_NUMBER];
	pthread_t consumers[C_NUMBER];
	// Create thread IDs
	size_t id_size;
	if(P_NUMBER > C_NUMBER)
	{ // If more producers, create producer number of IDs
		id_size = P_NUMBER;
	} else { // Else use consumer numer of IDs
		id_size = C_NUMBER;
	}
	size_t tID[id_size];
	for(size_t i = 1; i <= (id_size); i++)
	{	// Initialize the IDs
		tID[i-1] = i;
	}
	// Initialize semaphores
	if(sem_init(&not_full, 0, (unsigned)BUFFER_SIZE))
	{	// If failed, print error
		fprintf(stderr, "Failed to initialize semaphore not_full\n");
		exit(-1);
	}
	if(sem_init(&not_empty, 0, (unsigned)0))
	{	// If failed, print error
		fprintf(stderr, "Failed to initialize semaphore not_empty\n");
		exit(-1);
	}
	for(int i = 0; i < P_NUMBER; i++)
	{	// Create all producer threads
		if(pthread_create(&producers[i], NULL, produce_run, (void*) &tID[i]))
		{	// If failed, print error
			fprintf(stderr, "Failed to create producer thread with ID: %d\n", i);
			exit(-1);
		}
	}
	for(int i = 0; i < C_NUMBER; i++)
	{	// Create all consumer threads
		if(pthread_create(&consumers[i], NULL, consume_run, (void*) &tID[i]))
		{	// If failed, print error
			fprintf(stderr, "Failed to create consumer thread with ID: %d\n", i);
			exit(-1);
		}
	}
	for(int i = 0; i < P_NUMBER; i++)
	{ // Wait for all producer threads
		if(pthread_join(producers[i], NULL))
		{	// If failed, print error
			fprintf(stderr, "Failed to join thread with ID: %d\n", i);
			exit(-1);
		}
	}
	for(int i = 0; i < C_NUMBER; i++)
	{	// Wait for all consumer threads
		if(pthread_join(consumers[i], NULL))
		{	// If failed, print error
			fprintf(stderr, "Failed to join thread with ID: %d\n", i);
			exit(-1);
		}
	}
	// Remove semaphores and mutex
	if(sem_destroy(&not_full))
	{	// If failed, print error
		fprintf(stderr, "Falied to destroy semaphore not_full\n");
		exit(-1);
	}
	if(sem_destroy(&not_empty))
	{	// If failed, print error
		fprintf(stderr, "Falied to destroy semaphore not_empty\n");
		exit(-1);
	}
	if(pthread_mutex_destroy(&counter_lock))
	{	// If failed, print error
		fprintf(stderr, "Falied to destroy mutex counter_lock\n");
		exit(-1);
	}

  return 0;
}
/////////////////////////////////  Functions  //////////////////////////////////
/*******************************************************************************
Function:		void* produce_run(void* tID)
Use:				Insert widget into buffer N_P_STEPS number of times
Arguments:	tID - ID of the thread running the function
Returns:		none
*******************************************************************************/
void* produce_run(void* tID){
	int *ID = tID;
	for(int i = 0; i < N_P_STEPS; i++)
	{	// Produce widget for each step
		sleep(1);
		if(sem_wait(&not_full))
		{	// If failed, print error
			fprintf(stderr, "Semaphore not_full failed to wait");
			exit(-1);
		}
		insert_widget(*ID);
		if(sem_post(&not_empty))
		{	// If failed, print error
			fprintf(stderr, "Semaphore not_empty failed to post");
			exit(-1);
		}
	}
	pthread_exit(0);
}
/*******************************************************************************
Function:		void* consume_run(void* tID)
Use:				Remove widget from buffer N_C_STEPS number of times
Arguments:	tID - ID of the thread running the function
Returns:		none
*******************************************************************************/
void* consume_run(void* tID){
	int *ID = tID;
	for(int i = 0; i < N_C_STEPS; i++)
	{	// Consume widget for each step
		sleep(1);
		if(sem_wait(&not_empty))
		{	// If failed, print error
			fprintf(stderr, "Semaphore not_empty failed to wait");
			exit(-1);
		}
		remove_widget(*ID);
		if(sem_post(&not_full))
		{	// If failed, print error
			fprintf(stderr, "Semaphore not_full failed to post");
			exit(-1);
		}
	}
	pthread_exit(0);
}
/*******************************************************************************
Function:		void insert_widget(int tID)
Use:				Inserts a widget into the buffer
Arguments:	tID - ID of the pthread calling the function
Returns:		none
*******************************************************************************/
void insert_widget(int tID){
	if(pthread_mutex_lock(&counter_lock))
	{	// If failed, print error
		fprintf(stderr, "Mutex counter_lock failed to lock");
		exit(-1);
	}
	widgets++;
	printf("Producer %d inserted a widget. Total is now %d\n", tID,
		(int)widgets);
	if(pthread_mutex_unlock(&counter_lock))
	{	// If failed, print error
		fprintf(stderr, "Mutex counter_lock failed to unlock\n");
		exit(-1);
	}
}
/*******************************************************************************
Function:		void remove_widget(int tID)
Use:				Removes a widget into the buffer
Arguments:	tID - ID of the pthread calling the function
Returns:		none
*******************************************************************************/
void remove_widget(int tID){
	if(pthread_mutex_lock(&counter_lock))
	{	// If failed, print error
		fprintf(stderr, "Mutex counter_lock failed to lock");
		exit(-1);
	}
	widgets--;
	printf("Consumer %d removed a widget. Total is now %d\n", tID,
		(int)widgets);
	if(pthread_mutex_unlock(&counter_lock))
	{	// If failed, print error
		fprintf(stderr, "Mutex counter_lock failed to unlock\n");
		exit(-1);
	}
}
