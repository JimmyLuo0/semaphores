#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>

#define KEY 24602
#define SEG_SIZE 200

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
};


int main(int argc, char *argv[]) {
  int semd;
  int v, r;
  char input[3];
  int shmd;
  char * data;
  int fd;

  if(!strncmp(argv[1], "-c", 30)){
    semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semd == -1) {
      printf("error %d: %s\n", errno, strerror(errno));
      semd = semget(KEY, 1, 0);
      v = semctl(semd, 0, GETVAL, 0);
      printf("semctl returned: %d\n", v);
    }
    else {
      union semun us;
      us.val = 1;
      r = semctl(semd, 0, SETVAL, us);
      printf("semctl returned: %d\n", r);
    }
    printf("semaphore created\n");

    shmd = shmget(KEY, SEG_SIZE, IPC_CREAT | 0644);
    data = shmat(shmd, 0, 0);
    printf("shared memory created\n");

    fd = open("textfile",O_CREAT|O_TRUNC,0644);
    printf("file created\n");
    close(fd);
  }

  if(!strncmp(argv[1], "-v", 30)){
    fd = open("textfile", O_RDONLY);
    char buff[100];
    read(fd, buff, 500);
    printf("the story so far:\n %s\n", buff);
    close(fd);
 }

  shmdt(data);

  if (!strncmp(argv[1], "-r", 30)) {
    shmctl(shmd, IPC_RMID, 0);
    printf("segment deleted\n");
  }

  return 0;
}
