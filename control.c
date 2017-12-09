#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <errno.h>

void print_file(char *file) {
	FILE *f = fopen(file, "r");
	if (!f) {
		return;
	}
	char input[256];
	while (fgets(input, sizeof(input), f)) {
		printf("%s", input);
	}
	fclose(f);
	printf("\n");
}

int main(int argc, char* argv[]) {
	
	umask(0000);
	
	int semKEY = 12345;
	int shmKEY = 13579;

	if (argv[1] == NULL) {
		printf("Please enter in an argument.\n");
		exit(1);
	}
	
	int semID = semget(semKEY, 1, IPC_CREAT | 0644);
	if (semID == -1) {
		printf("Error: %s\n", strerror(errno));
		return 1;
	}
	
	if (strcmp(argv[1], "-c") == 0) {
		//Should make the shared memory, semaphore and file (open the file with the truncate flag). 
		//Set any values that are needed.
		int shm = shmget(shmKEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
		if (shm == -1) {
			printf("Error: %s\n", strerror(errno));
			return 1;
		}
		
		int fd = open("story.txt", O_CREAT | O_TRUNC, 0644);
		if (fd == -1) {
			printf("Error: %s\n", strerror(errno));
			return 1;
		}
		
		union semun value;
		value.val = 1;

		semID = semctl(semID, 0, SETVAL, value);
		if (semID == -1) {
			printf("Error: %s\n", strerror(errno));
			return 1;
		}

		close(fd);
		printf("story.txt created!\n");
	}
	
	if (strcmp(argv[1], "-v") == 0) {
		// Output the contents of the story file. 
		print_file("story.txt");
	}
	
	if (strcmp(argv[1], "-r") == 0) {
		//Remove the shared memory and the semaphore
		//Display the full contents of the story.
		
		semID = semget(semKEY, 1, 0600);
		semID = semctl(semID, 0, IPC_RMID);
		if (semID == -1) {
			printf("Error: %s\n", strerror(errno));
			return 1;
		}

		int shm = shmget(shmKEY, sizeof(int), 0);
		if (shm == -1) {
			printf("Error: %s\n", strerror(errno));
			return 1;
		}
		
		struct shmid_ds buf;
		shm = shmctl(shm, IPC_RMID, &buf);
		if (shm == -1) {
			printf("Error: %s\n", strerror(errno));
			return 1;
		}
		
		printf("shared memory deleted: %d\n", shm);
		printf("semaphone deleted: %d\n", semID);
		print_file("story.txt");
		
	}
	
	return 0;
	
}