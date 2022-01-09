#include "common.h"
#include "syscall.h"
#include "fs.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
    case SYS_exit:
      _halt(a[1]);
      return 0;
    break;
    case SYS_yield:
      _yield();
      return 0;
    case SYS_write:
    if(a[1]==1||a[1]==2){
        for(int i=0;i<a[3];i++){
          _putc(((char *)a[2])[i]);
        }
        return a[3];
      }
      return -1;
    break;
    case SYS_brk:
      // /program_break = a[1];
      return 0;
    break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
