#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 12

pthread_mutex_t mutexLastTrame = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexStates[N];
pthread_mutex_t mutexNewValue = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condRead = PTHREAD_COND_INITIALIZER;
pthread_cond_t condWrite = PTHREAD_COND_INITIALIZER;

typedef enum {
	READING = 1,
	READ,
	NOTREAD,
	WRITING
} State;
int lastTrame = -1;
int buffer[N];
State states[N];
int nbElements;

void* productor(void* nb);
void* consumer(void* nb);
int write(int v);
int startRead(int min);
int read(int indice);
void endRead(int indice);

void* productor(void* nb) {
	int* ptr = nb;
	int a = *ptr;
	int value = 0;

	while(1) {
		int indice = write(value);
		printf("[%d] buffer[%d] = %d\n", a, indice, value);
		sleep(1);
		value++;
	}
}

void* consumer(void* nb) {
	int* ptr = nb;
	int a = *ptr;

	int indCons;
	while(1) {
		/*printf("start writing...\n");*/
		indCons = startRead(lastTrame);
		printf("reading indice = %d\n", indCons);

		pthread_mutex_lock(&mutexLastTrame);
		lastTrame = read(indCons);
		pthread_mutex_unlock(&mutexLastTrame);

		endRead(indCons);

		sleep(1);
	}
}

int write(int v) {
	int ind = 0;
	int min = -1;
	int i = 0;

	for(i=0; i<N; i++) {
		pthread_mutex_lock(&mutexStates[i]);
		if(states[i] == READ) {
			if(min != -1) {
				pthread_mutex_unlock(&mutexStates[ind]);
			}
			ind = i;
			break;
		} else if(states[i]==NOTREAD && (min==-1 || buffer[i]<min)) {
			if(min != -1) {
				pthread_mutex_unlock(&mutexStates[ind]);
			}
			min = buffer[i];
			ind = i;
		} else {
			pthread_mutex_unlock(&mutexStates[i]);
		}
	}

	states[ind] = WRITING;
	buffer[ind] = v;
	states[ind] = NOTREAD;
	pthread_mutex_unlock(&mutexStates[ind]);
	pthread_cond_signal(&condRead);

	return ind;
}

int startRead(int min) {
	int res = 0;

	int i;
	int newValue = 0;
	do {
		for(i=0; i<N; i++) {
			pthread_mutex_lock(&mutexStates[i]);

			if(states[i]==NOTREAD && (min==-1 || buffer[i]<min)) {
				if(min != -1) {
					pthread_mutex_unlock(&mutexStates[res]);
				}
				min = buffer[i];
				res = i;
				newValue = 1;
			} else {
				pthread_mutex_unlock(&mutexStates[i]);
			}
		}
		if(!newValue) {
			/*printf("waiting for a new value...\n");*/
			pthread_cond_wait(&condRead, &mutexNewValue);
		}
	} while(!newValue);

	return res;
}

int read(int indice) {
	states[indice] = READING;
	int res = buffer[indice];
	pthread_mutex_unlock(&mutexStates[indice]);
	return res;
}

void endRead(int indice) {
	pthread_mutex_lock(&mutexStates[indice]);
	states[indice] = READ;
	pthread_mutex_unlock(&mutexStates[indice]);
}

int main(int argc, char* argv[]) {
	pthread_mutexattr_t attrMutex[N];       

	int i;
	for(i=0; i<N; i++) {
		states[i] = READ;
		pthread_mutexattr_init(&attrMutex[i]);
		pthread_mutex_init(&(mutexStates[i]), &attrMutex[i]);
	}

	int thr_id;
	pthread_t p_thread[1000];
	int nbThread = 0;
	pthread_attr_t attr;
	int action;

	do {
		printf("Choose an action to perform:\n");
		printf("1 - Create a new productor\n");
		printf("2 - Create a new consumer\n");
		printf("x - Exit\n");
		scanf(" %d", &action);
		switch(action) {
			case 1:
				pthread_attr_init(&attr);
				thr_id = pthread_create(&(p_thread[nbThread]), &attr, productor, (void*)&nbThread);
				nbThread++;
				break;
			case 2:
				pthread_attr_init(&attr);
				thr_id = pthread_create(&(p_thread[nbThread]), &attr, consumer, (void*)&nbThread);
				nbThread++;
				break;
			default:
				return 0;
		}
		action = 0;
	} while(1);
	return 0;
}