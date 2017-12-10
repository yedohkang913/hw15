#include "headers.h"

// Will attempt to access the resources using the semaphore
//Once in, it should display the last line added to the file (use shared memory to contain the size of the last line).
//Prompt the use for the next line.
//Once a new line is read, write that to the file, update the shared memory and then release the semaphore

int main() {
	
  int semID = semget(semKEY, 1, IPC_CREAT | 0644);
  if (semID == -1) {
    printf("Error: %s\n", strerror(errno));
    return 1;
  }
  
  //printf("1. test\n");
  
  struct sembuf sb_down;
  sb_down.sem_num = 0;
  sb_down.sem_flg = SEM_UNDO;
  sb_down.sem_op = -1;
  
  struct sembuf sb_up;
  sb_up.sem_num = 0;
  sb_up.sem_flg = SEM_UNDO;
  sb_up.sem_op = 1;
	
  //printf("hello\n");
	
  //semop(semID, &sb_down, 1);
  
  // printf("2. test\n");
  
  int shm = shmget(shmKEY, sizeof(int), 0644);
  if (shm == -1) {
    printf("Error: %s\n", strerror(errno));
    return 1;
  }
  
  int *length = shmat(shm, 0, 0);
  if (*length == -1) {
    printf("Error: %s\n", strerror(errno));
    return 1;
  }
  
  int fd = open("story.txt", O_CREAT | O_RDWR | O_APPEND, 0644);
  if (fd == -1) {
    printf("Error: %s\n", strerror(errno));
    return 1;
  }
  
  lseek(fd, -(*length), SEEK_END);
  char *buffer = (char *) calloc(*length + 1, sizeof(char));
  read(fd, buffer, *length);
  printf("%s", buffer);
  
  printf("Input next line: ");
  char input[256];
  fgets(input, 256, stdin);
  
  write(fd, input, strlen(input));
  close(fd);
  *length = strlen(input);
  shmdt(length);
  
  sb_up.sem_op = 1;
  semop(semID, &sb_up, 1);
  
  return 0;
}
