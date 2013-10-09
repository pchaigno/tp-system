#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>

int init(key_t key, int n) {
	int semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
	if(semid == -1) {
		if(errno != EEXIST) {
			return -1;
		}
		semid = semget(key, 0, 0);
		return semid;
	}
	int result = semctl(semid, 0, SETVAL, n);
	if(result == -1) {
		printf("Erreur lors du semctl");
		return -1;
	}
	return semid;
}

void delete(int semid) {
	int result = semctl(semid, 0, IPC_RMID, 0);
	if(result == -1) {
		printf("Erreur lors du semctl");
	}
}

void p(int semid) {
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

void v(int semid) {
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = 1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

void main1() {
	int sem = init(10, 0);
 	int pidFils = fork();
 	if(pidFils != 0){
	  p(sem);
	  printf("pere.");
	  //delete(sem);
	} else {
		sleep(2);
		printf("fils.");
		v(sem);
	}
}

void main2(int argc, const char* argv[] ) {
	int semid = init(4655, 1);
	if(semid == -1) {
		printf("Erreur a la creation du semaphore.");
		return;
	}
	p(semid);
	printf("debut : %s\n", argv[1]);
	sleep(5);
	printf("fin : %s\n", argv[1]);
	v(semid);
}



void fusion(int id, int deb1, int fin1, int fin2) {
  int tableau[] = (int *) shmat(id, NULL, 0);
  int *table1;
  int deb2=fin1+1;
  int compt1=deb1;
  int compt2=deb2;
  int i;
  
  table1=malloc((fin1-deb1+1)*sizeof(int));

  //on recopie les éléments du début du tableau
  for(i=deb1;i<=fin1;i++)
      {
      table1[i-deb1]=tableau[i];
      }
                  
  for(i=deb1;i<=fin2;i++)
      {        
      if (compt1==deb2) //c'est que tous les éléments du premier tableau ont été utilisés
          {
          break; //tous les éléments ont donc été classés
          }
      else if (compt2==(fin2+1)) //c'est que tous les éléments du second tableau ont été utilisés
          {
          tableau[i]=table1[compt1-deb1]; //on ajoute les éléments restants du premier tableau
          compt1++;
          }
      else if (table1[compt1-deb1]<tableau[compt2])
          {
          tableau[i]=table1[compt1-deb1]; //on ajoute un élément du premier tableau
          compt1++;
          }
      else
          {
          tableau[i]=tableau[compt2]; //on ajoute un élément du second tableau
          compt2++;
          }
      }
  free(table1);
 }
        

void tri_fusion_bis(int id,int deb,int fin) {
  if (deb!=fin) {
    int milieu=(fin+deb)/2;
    int semid = init(milieu+1100, 0);
    
    int pidFils = fork();
 		if (pidFils!=0){
   		tri_fusion_bis(id,deb,milieu);
   		v(semid);
   		exit(0);
  	} else {
  		int pidFils = fork();
	 	if (pidFils!=0){
		 	tri_fusion_bis(id,milieu+1,fin);
		 	v(semid);
		 	exit(0);
  		} else {
  			p(semid);
  			p(semid);
  			fusion(id,deb,milieu,fin);
  		}
  	}
    
  }
}

void tri_fusion(int tableau[],int longueur) {
	if (longueur>0) {
		int id = shmget(2105, sizeof(int)*(tableau.length), IPC_CREAT);
		if (t == -1) {
			printf("Erreur lors de la cration de la mem partagee.");	
		}
		int * tab = shmat(id, NULL, 0);
		int i;
		for (i = 0; i < tableau.length; i++) {
			tab[i] = tableau[i];
		}
		tri_fusion_bis(id, 0, longueur-1);
	}
}
