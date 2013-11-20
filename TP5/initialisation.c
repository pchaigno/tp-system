#include "semaphore.h"

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


void main() {
	// Init memoire partagee
	int dataid = shmget(DATAKEY, sizeof(int) * SIZE, IPC_CREAT | 0666);
	if(dataid == -1) {
		perror("erreur sur shmget de dataid");
	}
	int queueid = shmget(QUEUEKEY, sizeof(int), IPC_CREAT | 0666);
	if(queueid == -1) {
		perror("erreur sur shmget de queueid");
	}
	int teteid = shmget(TETEKEY, sizeof(int), IPC_CREAT | 0666);
	if(teteid == -1) {
		perror("erreur sur shmget de teteid");
	}
	
	int *tete_ptr = (int*)shmat(teteid, NULL, 0);
	if((int)tete_ptr == -1) {
		printf("Erreur a l'initialisation de la tete.");
	}
	*tete_ptr = 0;
	int *queue_ptr = (int*)shmat(queueid, NULL, 0);
	if((int)queue_ptr == -1) {
		printf("Erreur a l'initialisation de la queue.");
	}
	*queue_ptr = 0;
	
	// Init semaphore
	int semvide = init(VIDE, SIZE);
	int semdata = init(DATA, 0);
	int mutexp = init(MUTEXP, 1);
	int mutexc = init(MUTEXC, 1);
}
