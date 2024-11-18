#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  int fds1[2], fds2[2];
  if (pipe(fds1) != 0 || pipe(fds2) != 0) {
    printf(2, "pipe failed\n");
    exit();
  }

  char c = '\0';
  if (write(fds1[1], &c, 1) != 1) {
    printf(2, "write failed\n");
    exit();
  }
  close(fds1[1]);

  int pid;
  if ((pid = fork()) == 0) {
    close(fds2[0]);

    if (read(fds1[0], &c, 1) != 1) {
      printf(2, "read failed (child)\n");
      exit();
    }
    close(fds1[0]);
    printf(1, "%d: received ping\n", getpid());

    if (write(fds2[1], &c, 1) != 1) {
      printf(2, "write failed (child)\n");
    }
    close(fds2[1]);

    exit();
  }

  close(fds1[0]);
  close(fds2[1]);

  wait();

  if (read(fds2[0], &c, 1) != 1) {
    printf(2, "read failed (parent)\n");
    exit();
  }
  close(fds2[0]);
  printf(1, "%d: received pong\n", getpid());

  exit();
}