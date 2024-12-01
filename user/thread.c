#include "types.h"
#include "stat.h"
#include "user.h"
#include "mmu.h"

int thread_create(void (*start_routine)(void *), void *arg) {
  void *ptr, *stack;

  if ((ptr = malloc(2 * PGSIZE)) == 0) {
    return -1;
  }
  stack = (void *)PGROUNDUP((uint)ptr + sizeof(uint));
  *(uint *)((uint)stack - sizeof(uint)) = (uint)ptr;
  return clone(start_routine, stack, arg);
}

int thread_join() {
  void *ptr, *stack;
  int pid;

  if ((pid = join(&stack)) == -1) {
    return -1;
  }
  ptr = (void *)(*(uint *)((uint)stack - sizeof(uint)));
  free(ptr);
  return pid;
}