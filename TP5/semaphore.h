#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>

#define DATAKEY 1281
#define QUEUEKEY 1291
#define TETEKEY 1301

#define MUTEXC 1311
#define MUTEXP 1341
#define VIDE 1321
#define DATA 1331

#define SIZE 5

int init(key_t key, int n);
void p(int semid);
void v(int semid);

void test();
