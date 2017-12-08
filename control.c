#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>


int main(int argc, char* argv[]) {
	
	umask(0000);
	
	int num = 0;
	int ds = 0;
	
	int KEY = 12345;

	if (argv[1] == NULL) {
		printf("Please enter in an argument.");
		exit(1);
	}
	
	if (strcmp(argv[1], "-c") == 0) {
		//Should make the shared memory, semaphore and file (open the file with the truncate flag). 
		//Set any values that are needed.
		int fd = open("story.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		int sm = shmget(KEY, sizeof(int), IPC_CREAT | IPC_EXCL);
		ds = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0600);
		if (ds == -1) {
			exit(1);
		}
		//num = atoi(argv[2]);
		/*
		if(!argv[2]){
			printf("-c requires a second command (integer)!\n");
			exit(0);
		}
		*/
		if (ds == -1) {
			printf("semaphore already exists\n");
			exit(1);
		}
		else {
			printf("semaphore created: %d\n", ds);
			//printf("value set: %d\n", semctl(ds, 0, GETVAL));
			semctl(ds, 0, SETVAL, 1);
			
		}
		printf("story.txt created!\n");
	}
	
	if (strcmp(argv[1], "-v") == 0) {
		//output contents of the story file
	  int fd1 = open("story.txt", O_RONLY, 0644);
	}
	
	if (strcmp(argv[1], "-r") == 0) {
		//Remove the shared memory and the semaphore
		//Display the full contents of the story.
		ds = semget(KEY, 1, 0600);
		printf("semaphore removed: %d\n", semctl(ds, 0, IPC_RMID));
		fd = shmctl(ds, IPC_RMID, NULL);
		printf("shared memory deleted: %d\n", fd);
		int fd2 = open("story.txt", O_RONLY, 0644);
		
	}
	
	return 0;
	
}