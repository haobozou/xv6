#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"

void lock_init(struct lock_t *lock) {
  lock->locked = 0;
}

void lock_acquire(struct lock_t *lock) {
  while (xchg(&lock->locked, 1) != 0)
    ;
  __sync_synchronize();
}

void lock_release(struct lock_t *lock) {
  __sync_synchronize();
  xchg(&lock->locked, 0);
}