#include "cpu/exec.h"
make_EHelper(auipc) {
  //rtl_sr(id_dest->reg, &id_src->val, 4);
  cpu.pc+=id_src;
  rtl_lr(id_dest,&cpu.pc,4);
  decinfo.is_jmp = true;
  print_asm_template2(lui);
}