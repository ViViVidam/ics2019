#include "common.h"
#include "syscall.h"
#include "fs.h"

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  int res=0;
  panic("Unhandled syscall ID = %d %d", a[0],SYS_open);
  switch (a[0]) {
    case SYS_exit:
      _halt(a[1]);
      return 0;
    break;
    case SYS_yield:
      _yield();
      return 0;
    break;
    case SYS_write:
      //_putc('1');
      res=fs_write(a[1],(void*)a[2],a[3]);
      break;
    case SYS_read:
      res=fs_read(a[1],(void*)a[2],a[3]);
      break;
    case SYS_open:
     _putc('2');
      res=fs_open((char*)a[1],a[2],a[3]);
       panic("Unhandled syscall ID = %d", a[0]);
      break;
    case SYS_close:
      res=fs_close(a[1]);
      break;
    case SYS_lseek:
      res=fs_lseek(a[1],a[2],a[3]);
      break;
    case SYS_brk:
      // /program_break = a[1];
    break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  c->GPRx=res;
  return 0;
}
