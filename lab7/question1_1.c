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
	x = (int *)shmat(IPC_PRIVATE, 0, 0);;
	*x = 0;

	if(fork() == 0){
		if(fork() == 0){
			for(i = 0; i < LOOP; i ++){
				*x += 3;
			}
		}
		else{
			for(j = 0; i < LOOP; i ++){
				*x += 2;
			}
		}
	}
	else {
		for(k = 0; i < LOOP; i ++){
				*x += 1;
		}
	}
	printf("%d\n", *x);
	return 0;
}