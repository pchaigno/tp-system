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

void test() {
	printf("hello\n");
}
