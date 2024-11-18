#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

void find(char *path, char *pattern) {
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    printf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    printf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type != T_DIR) {
    printf(2, "find: %s is not a directory\n", path);
    close(fd);
    return;
  }

  if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
    printf(1, "find: path too long\n");
    close(fd);
    return;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if (stat(buf, &st) < 0) {
      printf(1, "find: cannot stat %s\n", buf);
      continue;
    }
    switch (st.type) {
    case T_FILE:
      if (strcmp(de.name, pattern) == 0)
        printf(1, "%s\n", buf);
      break;
    case T_DIR:
      if ((strcmp(de.name, ".") != 0) && (strcmp(de.name, "..") != 0))
        find(buf, pattern);
      break;
    }
  }

  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf(2, "Usage: %s dir pattern\n", argv[0]);
    exit();
  }

  find(argv[1], argv[2]);
  exit();
}