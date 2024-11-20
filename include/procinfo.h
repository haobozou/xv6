struct procinfo_t {
  char st[3];
  int pid;
  struct procinfo_t *parent;
  char name[16];
};