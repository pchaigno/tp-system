#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ID 12345

void fusion(int tableau[], int deb1, int fin1, int fin2) {
	int *table1;
	int deb2 = fin1+1;
	int compt1 = deb1;
	int compt2 = deb2;
	int i;

	table1 = malloc((fin1-deb1+1) * sizeof(int));

	// On recopie les éléments du début du tableau
	for(i=deb1; i<=fin1; i++) {
		table1[i-deb1] = tableau[i];
	}

	for(i=deb1; i<=fin2; i++) {        
		if(compt1 == deb2) {
			// Tous les éléments du premier tableau ont été utilisés
			break; // Tous les éléments ont donc été classés
		} else if(compt2 == fin2+1) {
			// Tous les éléments du second tableau ont été utilisés
			tableau[i] = table1[compt1-deb1]; // On ajoute les éléments restants du premier tableau
			compt1++;
		} else if(table1[compt1-deb1] < tableau[compt2]) {
			tableau[i] = table1[compt1-deb1]; // On ajoute un élément du premier tableau
			compt1++;
		} else {
			tableau[i] = tableau[compt2]; // On ajoute un élément du second tableau
			compt2++;
		}
	}
	free(table1);
}

void tri_fusion_bis(int tableau[], int deb, int fin) {
	if(deb != fin) {
		int i;
		printf("Process %d (father %d): ", getpid(), getppid());
		for(i=deb; i<fin+1; i++) {
			printf("%d - ", tableau[i]);
		}
		printf("\n");

		int milieu = (fin+deb)/2;
		if(fork() == 0) {
			/*int* tab_p = (int*)shmat(id, NULL, 0);
			tableau = tab_p;*/
			tri_fusion_bis(tableau, deb, milieu);
			exit(0);
		} else {
			if(fork() == 0) {
				/*int* tab_p = (int*)shmat(id, NULL, 0);
				tableau = tab_p;*/
				tri_fusion_bis(tableau, milieu+1, fin);
				exit(0);
			} else {
				wait();
				wait();	
				fusion(tableau, deb, milieu, fin);
				printf("Process %d (father %d) fusion: ", getpid(), getppid());
				for(i=deb; i<fin+1; i++) {
					printf("%d - ", tableau[i]);
				}
				printf("\n");
			}
		}
	}
}

void tri_fusion(int tableau[], int longueur) {
	if(longueur > 0) {
		tri_fusion_bis(tableau, 0, longueur-1);
	}
}

int main(int argc, char * argv[]) {
	int i;
	int taille = 10;
	int id = shmget(12345, sizeof(int)*taille, IPC_CREAT | 0777);
	int* tab_p = (int*)shmat(id, NULL, 0); 
	int tab[10] = {0, 2, 4, 3, 8, 9, 5, 1, 7, 6};
	for(i=0; i<10; i++) {
		tab_p[i] = tab[i];
	}

	printf("Debut: ");
	for(i=0; i<taille; i++) {
		printf("%d - ", tab_p[i]);
	}
	printf("\n");

	tri_fusion(tab_p, taille);

	printf("Fin: ");
	for(i=0; i<taille; i++) {
		printf("%d - ", tab_p[i]);
	}
	printf("\n");

	return 0;
}
