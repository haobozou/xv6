# Initial process execs /init.
# This code runs in user space.

#include "syscall.h"
#include "traps.h"
#include "memlayout.h"


# exec(init, argv)
.globl start
start:
  pushl $argv
  pushl $init
  pushl $MINADDR  // where caller pc would be
  movl $SYS_exec, %eax
  int $T_SYSCALL

# for(;;) exit();
exit:
  movl $SYS_exit, %eax
  int $T_SYSCALL
  jmp exit

# char init[] = "/init\0";
init:
  .string "/init\0"

# char *argv[] = { init, 0 };
.p2align 2
argv:
  .long init
  .long 0