#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void delete(int id);

int init(key_t cle, int nbsems, int value);

void P(int id);

void V(int id);