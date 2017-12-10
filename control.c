#include "headers.h"

void print_file(char *file) {
  FILE *f = fopen(file, "r");
  if (!f) {
    printf("Error: %s\n", strerror(errno));
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
  
  if (argv[1] == NULL) {
    printf("Please enter in an argument.\n");
    exit(1);
  }
  
  int shmID;
  int semID = semget(semKEY, 1, IPC_CREAT | 0644);
  if (semID == -1) {
    printf("Error: %s\n", strerror(errno));
    return 1;
  }
  
  if (strcmp(argv[1], "-c") == 0) {
    //Should make the shared memory, semaphore and file (open the file with the truncate flag). 
    //Set any values that are needed.
    int shmID = shmget(shmKEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
    if (shmID == -1) {
      printf("Error: %s\n", strerror(errno));
      return 1;
    }
    
    int fd = open("story.txt", O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
      printf("Error: %s\n", strerror(errno));
      return 1;
    }

    //semun kept giving me an error when compiling?
    int value = 1;
    //union semun value;
    // value.val = 1;
    
    if ((semctl(semID, 0, SETVAL, value)) == -1) {
      printf("Error: %s\n", strerror(errno));
      return 1;
    }
    
    close(fd);
    printf("story.txt created!\n\n");
    printf("semaphore created: %d\n", semID);
    printf("shared memory created: %d\n", shmID);
  }
  
  if (strcmp(argv[1], "-v") == 0) {
    // Output the contents of the story file.
    FILE *f = fopen("story.txt", "r");
    if(f){
      printf("Entire story:\n");
    }
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
    
    shmID = shmget(shmKEY, sizeof(int), 0);
    if (shmID == -1) {
      printf("Error: %s\n", strerror(errno));
      return 1;
    }
    
    struct shmid_ds buf;
    shmID = shmctl(shmID, IPC_RMID, &buf);
    if (shmID == -1) {
      printf("Error: %s\n", strerror(errno));
      return 1;
    }
    
    printf("shared memory deleted: %d\n", shmID);
    printf("semaphone deleted: %d\n", semID);
    printf("\nEntire story:\n");
    print_file("story.txt");
    remove("story.txt");
  }
  
  return 0;
  
}
