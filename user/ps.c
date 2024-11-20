#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "procinfo.h"

int main(int argc, char *argv[]) {
  int max;
  if (argc < 2) {
    max = NPROC;
  } else {
    max = atoi(argv[1]);
  }

  if (max > NPROC) {
    printf(2, "Number of entries to display must be at most %d\n", NPROC);
    exit();
  }

  int n;
  struct procinfo_t ptable[NPROC];
  if ((n = procinfo(max, ptable)) < 0) {
    printf(2, "Error: procinfo failed\n");
    exit();
  }

  printf(1, "PID\tPPID\tState\tName\n");
  for (int i = 0; i < n; i++) {
    if (ptable[i].parent) {
      printf(1, "%d\t%d\t%s\t%s\n", ptable[i].pid, ptable[i].parent->pid, ptable[i].st, ptable[i].name);
    } else {
      printf(1, "%d\t-\t%s\t%s\n", ptable[i].pid, ptable[i].st, ptable[i].name);
    }
  }

  exit();
}