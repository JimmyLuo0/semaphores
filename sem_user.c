#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define KEY 24602


int main() {

  int semd;
  int r;
  int v;
  int fd;
  char * input;

  semd = semget(KEY, 1, 0);
  struct sembuf sb;
  sb.sem_num = 0;
  //sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;

  semop(semd, &sb, 1);

  fd = open("textfile", O_WRONLY|O_TRUNC);
  printf("Last addition:\n");
  printf("Your addition: ");
  fgets(input, 25, stdin);
  write(fd, input, 25);
  // printf("got the semaphore!\n");
  // sleep(10);

  sb.sem_op = 1;
  semop(semd, &sb, 1);

  return 0;
}
