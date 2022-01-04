#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

make_EHelper(auipc) {
  int tmp = cpu.pc + id_src->val;
  printf("auipc %d %x %x\n",id_dest->reg,cpu.pc,tmp);
  rtl_sr(id_dest->reg,&tmp,4);
}

make_EHelper(addi){
  uint32_t tmp = 0;
  rtl_add(&id_dest->val,&id_src->val,&id_src2->val);
  rtl_sr(id_dest->reg,&id_dest->val,4);
  printf("addi %x %x %d\n",id_dest->reg,id_src->val,id_src2->val);
}