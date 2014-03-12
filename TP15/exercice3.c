#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int i;
	omp_set_num_threads(6);
	#pragma omp parallel for
		for(i=1; i<20; i++) {
			printf("Thread %d: i=%d\n", omp_get_thread_num(), i);
		}
	printf("End!\n");
}