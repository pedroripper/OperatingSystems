#include <sys/sem.h> 
#include <unistd.h> 
#include <stdio.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <stdlib.h> 
#include <ctype.h>

#define LOOP 500

int main(){
	int i, j, k;
	int *x;
	int shmemXkey = shmget(IPC_PRIVATE, sizeof(int),
		IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	x = (int *)shmat(shmemXkey, 0, 0);;
	*x = 0;

	if(fork() == 0){
		if(fork() == 0){
			for(i = 0; i < LOOP; i ++){
				*x += 3;
			}
			exit(0);
		}
		else{
			for(j = 0; j < LOOP; j ++){
				*x += 2;
			}
		exit(0);
		}
	}
	else {
		for(k = 0; k < LOOP;  k++){
				*x += 1;
		}
	}
	printf("%d\n", *x);
	return 0;
}