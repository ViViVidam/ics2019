#include "common.h"
#include "syscall.h"

extern end;

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
      if(a[1]==1||2){
        int i = (int) a[3];
        char* buffer = (char *)a[2];
        while(i--){
          _putc(*buffer);
          buffer++;
        }
        return buffer-((char*)a[2]);
      }
      else
        return -1;
    break;
    case SYS_brk:
      if(a[1]==0)
        return end;
      else if(end==a[1])
        return end+a[2];
    break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
