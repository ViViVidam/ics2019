#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

make_EHelper(auipc) {
  int32_t tmp = cpu.pc + id_src;
  printf("auipc cpu:%x %d\n",cpu.pc,id_dest->reg);
  rtl_sr(id_dest->reg,&tmp,4);
}

make_EHelper(addi){
  printf("exec_addi called\n");
  rtl_add(&id_dest->reg,&id_src->val,&id_src2->val);
}