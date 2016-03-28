#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define SEMKEY 390
#define SHMKEY 390

#define NUM_SEMS 3
#define S 0
#define SC 1
#define SH 2

// define any constants here
#define K 1024

union semun {
	unsigned short *array;
};

//shared data structure among processes
struct common {
	int waiting_c;
	int waiting_h;
};

void semWait(int semid, int semaphore) {
	struct sembuf psembuf;

	psembuf.sem_op = -1;
	psembuf.sem_flg = 0;
	psembuf.sem_num = semaphore;
	semop(semid,&psembuf,1);
	return;
}

void semSignal(int semid, int semaphore) {
	struct sembuf vsembuf;

	vsembuf.sem_op = 1;
	vsembuf.sem_flg = 0;
	vsembuf.sem_num = semaphore;
	semop(semid,&vsembuf,1);
	return;
}







