#include<sys/sem.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>


union semun{
int val;
struct semid_ds *buf;
ushort *array;
};
// inicializa o valor do semáforo
int setSemValue(int semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V 
int semaforoV(int semId);

int main (){
	int i, j, k;
	int *x;
	int semId;
	semId = semget (8752, 1, 0666 | IPC_CREAT);
	setSemValue(semId);
	int shmemXkey = shmget(IPC_PRIVATE, sizeof(int),
		IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	x = (int *)shmat(shmemXkey, 0, 0);;
	*x = 0;

	
	if(fork() == 0){
		if(fork() == 0){
			for(i=0; i<500; i++){
				semaforoP(semId);
				
				*x += 3;
				semaforoV(semId);
				
			}
			exit(0);
		} else {
			for(j=0; j<500; j++){
				semaforoP(semId);

				*x += 2;
				semaforoV(semId);
			}
			exit(0);
		}
	} else {
		for(k=0; k<500; k++){
			semaforoP(semId);

			*x += 1;
			semaforoV(semId);
		}
	}
	
	wait(NULL);
		delSemValue(semId);
	printf("Valor final %d\n", *x);
	return 0;
}



int setSemValue(int semId){
	union semun semUnion;
	semUnion.val = 1;
	return semctl(semId, 0, SETVAL, semUnion);
}
void delSemValue(int semId){
	union semun semUnion; 
	semctl(semId, 0, IPC_RMID, semUnion);
}
int semaforoP(int semId){
	struct sembuf semB;
	semB.sem_num = 0;
	semB.sem_op = -1;
	semB.sem_flg = SEM_UNDO;
	semop(semId, &semB, 1);
	return 0;
}
int semaforoV(int semId){
	struct sembuf semB;
	semB.sem_num = 0;
	semB.sem_op = 1;
	semB.sem_flg = SEM_UNDO;
	semop(semId, &semB, 1);
	return 0;
}