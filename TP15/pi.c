#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int i, result = 0;
	for(i=0; i<1000; i++) {
		result += submain();
	}
	return result;
}

int submain() {
	static long nb_pas = 10000000;
	double pas;
	int i;
	double x, pi, som = 0.0;
	pas = 1.0 / (double)nb_pas;
	omp_set_num_threads(24);
	#pragma omp parallel for reduction(+: som) private(x)
	for(i=1; i<=nb_pas; i++) {
		x = (i - 0.5) * pas;
		som = som + 4.0 / (1.0 + x*x);
	}
	pi = pas * som;
	//printf("PI=%0.10f\n", pi);
	return 0;
}