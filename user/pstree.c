#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "procinfo.h"

int *get_children(struct procinfo_t *info, int n, int ppid) {
  int *children;
  if ((children = malloc(n * sizeof(int))) == 0) {
    printf(2, "Error: malloc failed\n");
    return 0;
  }

  int current = 0;
  for (int i = 0; i < n; i++) {
    if (info[i].parent && info[i].parent->pid == ppid) {
      children[current] = i;
      current++;
    }
  }
  children[current] = 0;

  return children;
}

void pstree(struct procinfo_t *info, int n, int cur, int level, int *pipe) {
  printf(1, "%d\t%s\t", info[cur].pid, info[cur].st);

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

  printf(1, "%s\n", info[cur].name);

  int *children;
  if ((children = get_children(info, n, info[cur].pid)) == 0) {
    exit();
  }
  for (int i = 0; children[i] != 0; i++) {
    if (children[i + 1] != 0) {
      pipe[level] = 1;
    } else {
      pipe[level] = 0;
    }
    pstree(info, n, children[i], level + 1, pipe);
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
  struct procinfo_t info[NPROC];
  if ((n = procinfo(max, info)) < 0) {
    printf(2, "Error: procinfo failed\n");
    exit();
  }

  int i;
  int pipe[NPROC] = {0};
  printf(1, "pid\tstate\tname\n");
  for (i = 0; i < n; i++) {
    if (info[i].pid == 1) {
      break;
    }
  }
  pstree(info, n, i, 0, pipe);

  exit();
}