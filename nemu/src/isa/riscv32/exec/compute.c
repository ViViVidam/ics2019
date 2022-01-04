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

make_EHelper(sltiu){
  uint32_t val1 = id_src->val;
  uint32_t val2 = id_src2->val;
  if(val1<val2){
    id_dest->val = 1;
  }
  else{
    id_dest->val = 0;
  }
  rtl_sr(id_dest->reg,&id_dest->val,4);
  printf("sltiu %d %x %x\n",id_dest->val,id_src->val,id_src2->val);
}

make_EHelper(add){
  rtl_add(&id_dest->val,&id_src->val,&id_src2->val);
  rtl_sr(id_dest->reg,&id_dest->val,4);
  printf("add %x %x %d\n",id_dest->reg,id_src->val,id_src2->val);
}

make_EHelper(sub){
  rtl_sub(&id_dest->val,&id_src->val,&id_src2->val);
  rtl_sr(id_dest->reg,&id_dest->val,4);
  printf("sub %x %x %d\n",id_dest->reg,id_src->val,id_src2->val);
}