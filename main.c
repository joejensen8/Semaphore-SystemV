#include "CH4.h"

time_t t;

void c(void);
void h(void);

int main(int argc, char const *argv[]) {

	// spawn processes to test semaphore program
	
	int semid, shmid;
	unsigned short seminit[NUM_SEMS];
	struct common *shared;
	union semun semctlarg;

	// get semaphore memory id
	if ((semid = semget(SEMKEY, NUM_SEMS, 0777 | IPC_CREAT)) < 0) {
		perror("semget");
		exit(EXIT_FAILURE);
	}

	seminit[S] = 1;
	seminit[SH] = 0;
	seminit[SC] = 0;
	semctlarg.array = seminit;

	//apply initialization
	if ((semctl(semid, NUM_SEMS, SETALL, semctlarg)) < 0) {
		perror("semctl");
		exit(EXIT_FAILURE);
	}

	//get shared memory id
	if ((shmid = shmget(SHMKEY, 1*K, 0777 | IPC_CREAT)) < 0) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}

	//retrieve pointer to shared data structure
	if ((shared = (struct common *)shmat(shmid, 0, 0)) < 0) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	// initialize shared data struture variables
	shared->waiting_h = 0;
	shared->waiting_c = 0;

	int retVal; // used to check the return value of fork()

	// Spawn all child process with fork() to test

	// spawn 20 H's, and 5 C's
	for (int i = 0; i < 5; i++) {
		if ((retVal = fork()) == 0) {
			c();
		} else if (retVal < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < 20; i++) {
		if ((retVal = fork()) == 0) {
			h();
		} else if (retVal < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
	}

	//wait for all processes to finish
	for (int i = 0; i < 25; ++i) {
		if (wait(0) < 0) {
			perror("wait");
			exit(EXIT_FAILURE);
		}
	}

	fflush(stdout);
	printf("All finished, cleaning up!\n");
	fflush(stdout);	

	//delete semaphores
	if (semctl(semid, NUM_SEMS, IPC_RMID, 0) < 0) {
		perror("semctl");
		exit(EXIT_FAILURE);
	}

	//delete shared memory
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(EXIT_FAILURE);
	}	

	return EXIT_SUCCESS;
}

void c(void) {
	execl("C", "C", 0);
	perror("execl");
	exit(EXIT_FAILURE);
}

void h(void) {
	execl("H", "H", 0);
	perror("execl");
	exit(EXIT_FAILURE);
}

