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
	int dataid = shmget(DATAKEY, sizeof(int) * SIZE, 0);
	int * data;
	data = (int *) shmat(dataid, NULL, 0);
	
	int queueid = shmget(QUEUEKEY, sizeof(int), 0);
	int * queue;
	queue = shmat(queueid, NULL, 0);	
	
	int teteid = shmget(TETEKEY, sizeof(int), 0);
	int * tete;
	tete = shmat(teteid, NULL, 0);
	
	int semplein = init(VIDE, 0);
	int semdata = init(DATA, 0);
	int mutex = init(MUTEXP, 0);
	
	while(1) {
		p(mutex);
		data[tete] = 1;
		tete = (tete+1)%SIZE;
		v(semdata);
		p(semplein);
		v(mutex);
		
	}		
}
