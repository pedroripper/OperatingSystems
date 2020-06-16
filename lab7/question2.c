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
	int i, j, k, w;
	char *buffer;
	int semId;
	semId = semget (8752, 1, 0666 | IPC_CREAT);
	setSemValue(semId);
	int segkey = shmget(IPC_PRIVATE, sizeof(int),
		IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	buffer = (char *)shmat(segkey, 0, 0);;
	
	j = 0;
	
	if(fork() == 0){

		for(i = 0; i <128; i++){
			semaforoP(semId);
			printf("Processo (%d) escrevendo no buffer\n", getpid());
			buffer[j] = 'A' +(char)((rand())%26);
			j ++;
			if(j==32){
				j = 0;
			}
			semaforoV(semId);
		}
	} else {
		for(k = 0; k <128; k++){

			semaforoP(semId);
			printf("Processo (%d) lendo no buffer\n", getpid());

			for(w = 0; w < 32; w ++){
				printf("%c ", buffer[w]);
			}
			printf("\n");
			semaforoV(semId);
		}
	}
	printf("\n");

	wait(NULL);
	delSemValue(semId);
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