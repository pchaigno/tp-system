#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	omp_set_num_threads(4);
	#pragma omp parallel
	{
		int thread_id = omp_get_thread_num();
		printf("Hello World from thread %d\n", thread_id);
	}
	printf("End!\n");
}