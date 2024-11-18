#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf(2, "Usage: trace <mask> <command>\n");
    exit();
  }

  int mask = atoi(argv[1]);
  trace(mask);

  int pid;
  if ((pid = fork()) == 0) {
    exec(argv[2], argv + 2);
    exit();
  }

  wait();
  exit();
}