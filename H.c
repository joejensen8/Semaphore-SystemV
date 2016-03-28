#include "CH4.h"

struct common *shared;

int main(int argc, char const *argv[]) {

	// H's semaphore algorithm here

	int semid, shmid; // semaphore memory id, shared memory id

	int pid = getpid();
	
	// get semaphore memory id
	if ((semid = semget(SEMKEY, NUM_SEMS, 0777 | IPC_CREAT)) < 0) {
		perror("semget");
		exit(EXIT_FAILURE);
	}

	//get shared memory id
	if ((shmid = shmget(SHMKEY, 1*K, 0777 | IPC_CREAT)) < 0) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}

	//get pointer to shared data structure
	if ((shared = (struct common *)shmat(shmid, 0, 0)) < 0) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	semWait(semid, S);

	fflush(stdout);
	printf("H [%d] arrives\n", pid);
	fflush(stdout);

	if (shared->waiting_h >= 3 && shared->waiting_c >= 1) {
		fflush(stdout);
		printf("\nH [%d] is combining to make CH4\n\n", pid);
		fflush(stdout);
		for (int i = 0; i < 3; i++) {
			semSignal(semid, SH);
		}
		shared->waiting_h -= 3;
		semSignal(semid, SC);
		shared->waiting_c -= 1;
		semSignal(semid, S);
	} else {
		shared->waiting_h += 1;

		fflush(stdout);
		printf("H [%d] is waiting. Current C #[%d], Current H #[%d].\n", pid, shared->waiting_c, shared->waiting_h);
		fflush(stdout);
		
		semSignal(semid, S);
		semWait(semid, SH);
	}	

	return EXIT_SUCCESS;
}
