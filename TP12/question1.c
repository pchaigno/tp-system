#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

#define N 12

pthread_mutex_t mutexStates[N];
pthread_mutex_t mutexNewValue = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condEmpty = PTHREAD_COND_INITIALIZER;

typedef enum {
	READING = 1,
	READ1, // Read by consumer 1.
	READ2, // Read by consumer 2.
	READ,
	NOTREAD,
	WRITING
} State;
int frames[N];
State states[N];
int nbElements;

void* productor(void* nb);
void* consumer(void* nb);
int write(int v);
int startRead(int idCons);
int read(int idCons, int indice);
void endRead(int idCons, int indice);

/**
 * Selects the oldest frame not read.
 * @param reading The id of the consumer, 0 if productor.
 * @return The id of the oldest frame, -1 if none found.
 */
int selectOldestFrame(int reading) {
	int oldestFrame = -1;
	int min = INT_MAX;
	int i;
	int ownState;
	if(reading == 2) {
		ownState = READ2;
	} else if(reading == 3) {
		ownState = READ1;
	}
	for(i=0; i<N; i++) {
		if((states[i]==NOTREAD 
			|| (reading && (states[i]==READING || states[i]==ownState))) 
			&& frames[i]<min) {
		// This value is older than the last one.
			oldestFrame = i;
			min = frames[oldestFrame];
		}
	}
	return oldestFrame;
}

/**
 * Productor loop.
 * @param nb The productor id.
 */
void* productor(void* nb) {
	int* ptr = nb;
	int idProd = *ptr;
	int value = 0;

	while(1) {
		int indice = write(value);
		printf("[%d] frames[%d] = %d\n", idProd, indice, value);
		value++;
	}
}

/**
 * Consumer loop.
 * @param nb The consumer id.
 */
void* consumer(void* nb) {
	int* ptr = nb;
	int idCons = *ptr;
	int frameToRead;
	while(1) {
		frameToRead = startRead(idCons);
		read(idCons, frameToRead);
		endRead(idCons, frameToRead);
	}
}

/**
 * Produces a new frame.
 * @param v The value to write.
 */
int write(int v) {
	int i;
	int oldestFrame = -1;
	// Search for a read or empty frame.
	for(i=0; i<N; i++) {
		if(states[i] == READ) {
			oldestFrame = i;
			break;
		}
	}
	if(oldestFrame == -1) {
		oldestFrame = selectOldestFrame(0);
	}
	
	pthread_mutex_lock(&mutexStates[oldestFrame]);
	states[oldestFrame] = WRITING;
	pthread_mutex_unlock(&mutexStates[oldestFrame]);
	
	frames[oldestFrame] = v;
	//sleep(2);
	
	pthread_mutex_lock(&mutexStates[oldestFrame]);
	states[oldestFrame] = NOTREAD;
	pthread_mutex_unlock(&mutexStates[oldestFrame]);
	
	pthread_cond_signal(&condEmpty);
	return oldestFrame;
}

/**
 * Finds a frame to read and lock it.
 * If no frame is available, it waits for one.
 * @param The id of the frame to read.
 */
int startRead(int idCons) {
	int idToRead = -1;
	int i;
	int newValue = 0;
	int min = INT_MAX;
	do {
		idToRead = selectOldestFrame(idCons);
		if(idToRead == -1) {
		// No new value, need to wait.
			pthread_cond_wait(&condEmpty, &mutexNewValue);
		} else {
			if(states[idToRead] == NOTREAD) {
				pthread_mutex_lock(&mutexStates[idToRead]);
				states[idToRead] = READING;
				pthread_mutex_unlock(&mutexStates[idToRead]);
			}
		}
	} while(idToRead == -1);
	return idToRead;
}

/**
 * Reads the frame and unlock it.
 * @param idCons The id of the consumer.
 * @param id The id of the frame to read.
 * @return The frame read.
 */
int read(int idCons, int id) {
	int frame = frames[id];
	printf("[%d] Read %d at %d.\n", idCons, frame, id);
	sleep(1);
	return frame;
}

/**
 * Ends the read process.
 * The state of the frame read is put to READ.
 * @param idCons The id of the current consumer.
 * @param id The id of the frame read.
 */
void endRead(int idCons, int id) {
	pthread_mutex_lock(&mutexStates[id]);
	if(states[id] == READING) {
		if(idCons == 2) {
			states[id] = READ1;
		} else {
			states[id] = READ2;
		}
	} else {
		states[id] = READ;
	}
	pthread_mutex_unlock(&mutexStates[id]);
}

/**
 * Initialises the global variables.
 */
void init() {
	pthread_mutexattr_t attrMutex[N];
	int i;
	for(i=0; i<N; i++) {
		states[i] = READ;
		pthread_mutexattr_init(&attrMutex[i]);
		pthread_mutex_init(&(mutexStates[i]), &attrMutex[i]);
	}
}

int main(int argc, char* argv[]) {
	init();
	pthread_t p_thread[1000];
	int nbThread = 0;
	pthread_attr_t attr;
	int action;

	// Create he productor:
	pthread_attr_init(&attr);
	pthread_create(&(p_thread[nbThread]), &attr, productor, (void*)&nbThread);
	nbThread++;

	do {
		printf("Choose an action to perform:\n");
		printf("1 - Create a new consumer\n");
		printf("x - Exit\n");
		scanf(" %d", &action);
		switch(action) {
			case 1:
				pthread_attr_init(&attr);
				pthread_create(&(p_thread[nbThread]), &attr, consumer, (void*)&nbThread);
				nbThread++;
				break;
			default:
				return 0;
		}
		action = 0;
	} while(1);
	return 0;
}