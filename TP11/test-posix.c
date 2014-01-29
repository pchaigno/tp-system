#include <stdio.h>
#include <pthread.h>

#define NUMBER_ITER 10

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
int a = 1;
int b = 2;

void* do_loop(void* data) {
	int i;
	int *value = data;
	for(i=0; i<NUMBER_ITER; i++) {
		if(*value == 1) {
			pthread_mutex_lock(&mutex1);
		} else {
			pthread_mutex_lock(&mutex2);
		}
		printf("%d\n", *value);
		if(*value == 1) {
			pthread_mutex_unlock(&mutex2);
		} else {
			pthread_mutex_unlock(&mutex1);
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	int thr_id;
	pthread_t p_thread;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_mutex_lock(&mutex2);
	thr_id = pthread_create(&p_thread, &attr, do_loop, (void*)&a);

	do_loop((void*)&b);

	return 0;
}