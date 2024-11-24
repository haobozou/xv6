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
  struct procinfo_t info[NPROC];
  if ((n = procinfo(max, info)) < 0) {
    printf(2, "Error: procinfo failed\n");
    exit();
  }

  printf(1, "pid\tppid\tstate\tname\tct\trt\tdelay\ttickets\n");
  for (int i = 0; i < n; i++) {
    if (info[i].parent) {
      printf(1, "%d\t%d\t%s\t%s\t%d\t%d\t%d\t%d\n",
             info[i].pid, info[i].parent->pid, info[i].st, info[i].name, info[i].ct, info[i].rt, info[i].delay, info[i].tickets);
    } else {
      printf(1, "%d\t-\t%s\t%s\t%d\t%d\t%d\t%d\n",
             info[i].pid, info[i].st, info[i].name, info[i].ct, info[i].rt, info[i].delay, info[i].tickets);
    }
  }

  exit();
}