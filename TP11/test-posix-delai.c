#include <stdio.h>
#include <pthread.h>

#define NUMBER_ITER 10

int a = 1;
int b = 2;

void* do_loop(void* data) {
	int i;
	int *value = data;
	for(i=0; i<NUMBER_ITER; i++) {
		printf("%d\n", *value);
		sleep(2);
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	int thr_id;
	pthread_t p_thread;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	thr_id = pthread_create(&p_thread, &attr, do_loop, (void*)&a);

	sleep(1);
	do_loop((void*)&b);

	return 0;
}