#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argv[]) {
	int valeur2 = 2000, valeur1 = 1000;
	omp_set_num_threads(4);
	#pragma omp parallel firstprivate(valeur2)
	{
		valeur1++;
		valeur2++;
		printf("Thread %d: v1=%d, v2=%d\n", omp_get_thread_num(), valeur1, valeur2);
	}
	printf("End!\n");
}