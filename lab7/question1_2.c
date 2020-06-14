#include <sys/sem.h> 
#include <unistd.h> 
#include <stdio.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <stdlib.h> 
#include <ctype.h>

#define LOOP 500


typedef union semaphore {
	int val;
	struct semid_ds *buf; 
	short *array;
} Semaphore;


int setSemaphoreValue(int semId) {
	Semaphore s;
	s.val = 1;
	return semctl(semId, 0, SETVAL, s);
}

void deleteSemaphoreValue(int semId){
	Semaphore s;
	semctl(semId, 0, IPC_RMID, s);
}

int semWait(int semId){
	struct sembuf semB; 
	semB.sem_num = 0; 
	semB.sem_op = -1; 
	semB.sem_flg = SEM_UNDO; 
	semop(semId, &semB, 1); 
	return 0;
}

int semSignal(int semId) {
	struct sembuf semB;
	semB.sem_num = 0;
	semB.sem_op = 1; 
	semB.sem_flg = SEM_UNDO; 
	semop(semId, &semB, 1); 
	return 0;
}


int main(){
	int i, j, k;
	int *x;
	int semId;
	semId = semget (8752, 1, 0666 | IPC_CREAT);
	setSemaphoreValue(semId);

	int shmemXkey = shmget(IPC_PRIVATE, sizeof(int),
		IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	x = shmat(shmemXkey, 0, 0);

	if(fork() == 0){
		if(fork() == 0){
			for(i = 0; i < LOOP; i ++){
				semWait(semId);
				*x += 3;
				sleep(rand()%3);
				semSignal(semId);
				sleep(rand()%2);
			}
		}
		else{
			for(i = 0; i < LOOP; i ++){
				semWait(semId);
				sleep(rand()%3);
				*x += 2;
				sleep(rand()%3);
				semSignal(semId);
				sleep(rand()%2);
			}
		}
	}
	else {
			for(i = 0; i < LOOP; i ++){
				semWait(semId);
				sleep(rand()%3);
				*x += 1;
				sleep(rand()%3);
				semSignal(semId);
				sleep(rand()%2);
			}
	}
	printf("%d\n", *x);
	return 0;
}