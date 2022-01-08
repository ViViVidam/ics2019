#ifndef __ARCH_H__
#define __ARCH_H__

struct _Context {
  uintptr_t epc;
  struct _AddressSpace *as;
  uintptr_t status,cause,gpr[30];
};

#define GPR1 gpr[17]
#define GPR2 gpr[0]
#define GPR3 gpr[0]
#define GPR4 gpr[0]
#define GPRx gpr[0]

#endif
