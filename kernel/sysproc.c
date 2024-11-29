#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "procinfo.h"
#include "pageinfo.h"

int sys_fork(void) {
  return fork();
}

int sys_exit(void) {
  exit();
  return 0; // not reached
}

int sys_wait(void) {
  return wait();
}

int sys_kill(void) {
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void) {
  return myproc()->pid;
}

int sys_sbrk(void) {
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void) {
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (myproc()->killed) {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void) {
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_halt(void) {
  outw(0x604, 0x2000);
  return 0;
}

int sys_trace(void) {
  int mask;

  if (argint(0, &mask) < 0)
    return -1;
  myproc()->tmask = mask;
  return 0;
}

int sys_procinfo(void) {
  int max;
  struct procinfo_t *p;

  if (argint(0, &max) < 0 || argptr(1, (void *)&p, max * sizeof(*p)) < 0)
    return -1;
  return procinfo(max, p);
}

int sys_pageinfo(void) {
  int addr;
  struct pageinfo_t *p;

  if (argint(0, &addr) < 0 || argptr(1, (void *)&p, sizeof(*p)) < 0)
    return -1;
  return pageinfo((void *)addr, p);
}

int sys_mmap(void) {
  int hint;
  int len;
  int prot;

  if (argint(0, &hint) < 0)
    return -1;
  if (argint(1, &len) < 0)
    return -1;
  if (argint(2, &prot) < 0)
    return -1;
  return (int)mmap((void *)hint, len, prot);
}

int sys_munmap(void) {
  int addr;
  int len;

  if (argint(0, &addr) < 0)
    return -1;
  if (argint(1, &len) < 0)
    return -1;
  return munmap((void *)addr, len);
}

int sys_clone(void) {
  int fn;
  int stack;
  int arg;

  if (argint(0, &fn) < 0)
    return -1;
  if (argint(1, &stack) < 0)
    return -1;
  if (argint(2, &arg) < 0)
    return -1;
  return clone((void *)fn, (void *)stack, (void *)arg);
}

int sys_join(void) {
  int stack;

  if (argint(0, &stack) < 0)
    return -1;
  return join((void **)stack);
}