#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "procinfo.h"

#define NULL ((void *)0)

int *get_children(struct procinfo_t *ptable, int n, int ppid) {
  int *children;
  if ((children = malloc(n * sizeof(int))) == NULL) {
    printf(2, "Error: malloc failed\n");
    return NULL;
  }

  int current = 0;
  for (int i = 0; i < n; i++) {
    if (ptable[i].parent && ptable[i].parent->pid == ppid) {
      children[current] = i;
      current++;
    }
  }
  children[current] = 0;

  return children;
}

void pstree(struct procinfo_t *ptable, int n, int cur, int level, int *pipe) {
  printf(1, "%d\t%s\t", ptable[cur].pid, ptable[cur].st);

  for (int i = 0; i < level - 1; i++) {
    if (pipe[i]) {
      printf(1, " |  ");
    } else {
      printf(1, "    ");
    }
  }

  if (level > 0) {
    printf(1, " \\_ ");
  }

  printf(1, "%s\n", ptable[cur].name);

  int *children;
  if ((children = get_children(ptable, n, ptable[cur].pid)) == NULL) {
    exit();
  }
  for (int i = 0; children[i] != 0; i++) {
    if (children[i + 1] != 0) {
      pipe[level] = 1;
    } else {
      pipe[level] = 0;
    }
    pstree(ptable, n, children[i], level + 1, pipe);
  }
  free(children);
}

int main(int argc, char *argv[]) {
  int max;
  if (argc < 2)
    max = NPROC;
  else
    max = atoi(argv[1]);

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

  int i;
  int pipe[NPROC] = {0};
  printf(1, "PID\tState\tName\n");
  for (i = 0; i < n; i++) {
    if (ptable[i].pid == 1) {
      break;
    }
  }
  pstree(ptable, n, i, 0, pipe);

  exit();
}