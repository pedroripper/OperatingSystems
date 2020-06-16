#include <unistd.h> 
#include <stdio.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h> 

#define BUFFER_MAX_SIZE 200

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int setSemValue(int semId)
{
    union semun semUnion;
    semUnion.val = 1;

    return semctl(semId, 0, SETVAL, semUnion);
}

void delSemValue(int semId)
{
    union semun semUnion;
    semctl(semId, 0, IPC_RMID, semUnion);
}

int semaforoP(int semId)
{
    struct sembuf semB;
    semB.sem_num = 0;
    semB.sem_op = -1;
    semB.sem_flg = SEM_UNDO;
    semop(semId, &semB, 1);

    return 0;
}

int semaforoV(int semId)
{
    struct sembuf semB;
    semB.sem_num = 0;
    semB.sem_op = 1;
    semB.sem_flg = SEM_UNDO;
    semop(semId, &semB, 1);
    return 0;
}

void bufWrite(char *buffer, int *nBuffer, char *string, int n) {
    int i;

    *nBuffer = 0;

    for (i = 0; i < n; i++) {
        buffer[*nBuffer] = string[i];
        *nBuffer = *nBuffer + 1;
    }
}

void bufRead(char *buffer, int nBuffer) {
    int i;

    for (i = 0; i < nBuffer; i++) {
        printf("%c", buffer[i]);
    } 
}

int main(int argc, char *argv[]) {
    int semId;
    int nToWrite, bufShmemKey, nBufShmemKey, senderShmemKey;

    if (argc > 1) {
        if (sscanf(argv[1], "%d,%d,%d", &bufShmemKey, &nBufShmemKey, &senderShmemKey) != 3)
            exit(0);
    } else {
        bufShmemKey = shmget(IPC_PRIVATE, sizeof(char) * BUFFER_MAX_SIZE,
            IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
        nBufShmemKey = shmget(IPC_PRIVATE, sizeof(int),
            IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
        senderShmemKey = shmget(IPC_PRIVATE, sizeof(int),
            IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    
        printf("shmemkeys: %d,%d,%d\n", bufShmemKey, nBufShmemKey, senderShmemKey);
    }
   
    int *sender = shmat(senderShmemKey, 0, 0);
    char *buffer = shmat(bufShmemKey, 0, 0);
    char *toWrite = (char *)malloc(sizeof(char) * BUFFER_MAX_SIZE);

    int *nBuffer = shmat(nBufShmemKey, 0, 0);
    *nBuffer = 0;

    semId = semget(1820299, 1, IPC_CREAT);

    if (semId == -1) {
        printf("Falha ao criar/obter semaforo!");
        exit(0);
    }

    while (1) {

        if (*sender != getpid()) {
            semaforoP(semId);

            if (*nBuffer > 0 ) {
                printf("(pid: %d) ", getpid());
                bufRead(buffer, *nBuffer);
                printf("\n");

                sleep(rand() % 3);
            }

            nToWrite = sprintf(toWrite, "hi from %d", getpid());
            bufWrite(buffer, nBuffer, toWrite, nToWrite);
            *sender = getpid();
        
            semaforoV(semId);

            sleep(rand() % 2);
        }
    }
}
