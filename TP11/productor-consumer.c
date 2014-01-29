#include <stdio.h>
#include <pthread.h>

#define N 4
#define NB_THREADS 100

pthread_mutex_t mutexPut;
pthread_mutex_t mutexGet;
pthread_mutex_t mutexNbElements;
pthread_mutex_t mutexBlockedThreads;
pthread_cond_t condFull;
pthread_cond_t condEmpty;
int blockedThreads[NB_THREADS];
int buffer[N];
int idProd, nbElements, head, queue, nbBlocked;

void removeBlockedThread() {
	int i;
	pthread_t idThread = pthread_self();
	int found = 0;
	pthread_mutex_lock(&mutexBlockedThreads);
	for(i=0; i<nbBlocked-1; i++) {
		if(blockedThreads[i] == idThread) {
			found = 1;
		}
		if(found) {
			blockedThreads[i] = blockedThreads[i+1];
		}
	}
	nbBlocked--;
	pthread_mutex_unlock(&mutexBlockedThreads);
}

void addBlockedThread() {
	pthread_t idThread = pthread_self();
	pthread_mutex_lock(&mutexBlockedThreads);
	blockedThreads[nbBlocked] = idThread;
	nbBlocked++;
	pthread_mutex_unlock(&mutexBlockedThreads);
}

int isFull() {
	pthread_mutex_lock(&mutexNbElements);
	int full = (nbElements == N);
	pthread_mutex_unlock(&mutexNbElements);
	return full;
}

int isEmpty() {
	pthread_mutex_lock(&mutexNbElements);
	int empty = (nbElements == 0);
	pthread_mutex_unlock(&mutexNbElements);
	return empty;
}

void put(int v) {
	pthread_mutex_lock(&mutexPut);

	int full = isFull();
	int empty = isEmpty();
	if(full) {
		addBlockedThread();
		pthread_cond_wait(&condFull, &mutexPut);
		removeBlockedThread();
	}

	buffer[queue] = v;
	pthread_mutex_lock(&mutexNbElements);
	nbElements++;
	pthread_mutex_unlock(&mutexNbElements);
	queue = (queue + 1) % N;

	if(empty) {
		pthread_cond_signal(&condEmpty);
	}

	pthread_mutex_unlock(&mutexPut);
}

int get() {
	pthread_mutex_lock(&mutexGet);

	int full = isFull();
	int empty = isEmpty();
	if(empty) {
		addBlockedThread();
		pthread_cond_wait(&condEmpty, &mutexGet);
		removeBlockedThread();
	}

	int value = buffer[head];
	buffer[head] = 0;
	pthread_mutex_lock(&mutexNbElements);
	nbElements--;
	pthread_mutex_unlock(&mutexNbElements);
	head = (head + 1) % N;

	if(full) {
		pthread_cond_signal(&condFull);
	}

	pthread_mutex_unlock(&mutexGet);
	return value;
}

void init() {
	nbElements = 0;
	head = 0;
	queue = 0;
	idProd = 0;
}

void* productor() {
	int i;
	int end = N / 2;
	int value = idProd;
	idProd++;
	for(i=0; i<end; i++) {
		put(value + 10);
		sleep(1);
	}
}

void* consumer() {
	int i, value, end = N / 2;
	for(i=0; i<end; i++) {
		value = get();
		sleep(1);
	}
}

void createProductor() {
	pthread_t p_thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&p_thread, &attr, productor, NULL);
}

void createConsumer() {
	pthread_t p_thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&p_thread, &attr, consumer, NULL);
}

void displaySystemState() {
	int i;
	printf("Nb blocked: %d\n", nbBlocked);
	for(i=0; i<nbBlocked; i++) {
		printf("%d ", blockedThreads[i]);
	}
	printf("\n");

	printf("head: %d\n", head);
	printf("queue: %d\n", queue);

	pthread_mutex_lock(&mutexPut);
	pthread_mutex_lock(&mutexGet);
	for(i=0; i<N; i++) {
		printf("%d ", buffer[i]);
	}
	pthread_mutex_unlock(&mutexPut);
	pthread_mutex_unlock(&mutexGet);
	printf("\n");
}

int main(int argc, char* argv[]) {
	init();

	int action;

	do {
		printf("Choose an action to perform:\n");
		printf("1 - Create a new productor.\n");
		printf("2 - Create a new consumer.\n");
		printf("3 - Display the system state.\n");
		printf("4 - Quit\n");
		scanf("%d", &action);

		switch(action) {
			case 1:
				createProductor();
				break;
			case 2:
				createConsumer();
				break;
			case 3:
				displaySystemState();
				break;
			case 4:
				break;
			default:
				printf("Incorrect action!\n");
		}

		printf("\n");
	} while(action != 4);

	return 0;
}