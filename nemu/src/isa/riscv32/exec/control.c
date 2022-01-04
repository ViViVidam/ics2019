#include "cpu/exec.h"
make_EHelper(auipc) {
  //rtl_sr(id_dest->reg, &id_src->val, 4);
  int32_t tmp = cpu.pc + id_src;
  printf("auipc %d\n",id_dest->reg);
  rtl_lr(id_dest->reg,&tmp,4);
  //print_asm_template2(lui);
}