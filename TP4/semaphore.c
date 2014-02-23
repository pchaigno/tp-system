#include "semaphore.h"

void delete(int id) {
	semctl(id, 0, IPC_RMID, 0);
}

int init(key_t cle, int nbsems, int value) {
	int id = semget(cle, nbsems, 0666 | IPC_CREAT | IPC_EXCL);
	int i = 0;
	union semun {
		int val;
		struct semid_ds *buf;
		ushort * array;
	} argument;
	argument.val = value;

	if(errno == EEXIST) {
		if ((id = semget(cle, 0, 0)) == -1) {
			perror("IPC error 1: semget"); exit(1);
		}
	} else {
		for(i=0; i<nbsems; i++) {
			if(semctl(id, i, SETVAL, argument) < 0 ) {
				fprintf(stderr, "Unable to initialize semaphore %d.\n", i);
				delete(id);
				exit(0);
			}
		}
	}
	return id;
}

void P(int id) {
	struct sembuf operation;
	operation.sem_num = 0;
	operation.sem_op = -1;
	operation.sem_flg = 0;

	semop(id, &operation, 1);
}

void V(int id) {
	struct sembuf operation;
	operation.sem_num = 0;
	operation.sem_op = 1;
	operation.sem_flg = 0;

	semop(id, &operation, 1);
}

int main(int argc, char * argv[]) {
	int id = init(1456, 1, 3);
	P(id);
	P(id);
	/*int id2 = init(1456, 1, 3);
	printf("%d", id2);*/
	P(id);
	P(id);
	V(id);
	P(id);
	delete(id);

	return 0;
}