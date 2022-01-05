#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

make_EHelper(auipc) {
  int tmp = cpu.pc + id_src->val;
  printf("auipc %x %x %x\n",id_dest->val,cpu.pc,tmp);
  rtl_sr(id_dest->reg,&tmp,4);
}

make_EHelper(addi){
  uint32_t tmp = 0;
  rtl_add(&id_dest->val,&id_src->val,&id_src2->val);
  rtl_sr(id_dest->reg,&id_dest->val,4);
  printf("addi %x %x %d\n",id_dest->val,id_src->val,id_src2->val);
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

make_EHelper(sltu){
  uint32_t rs1 = id_src->val;
  uint32_t rs2 = id_src2->val;
  if(rs1<rs2){
    id_dest->val = 1;
  }
  else{
    id_dest->val = 0;
  }
  rtl_sr(id_dest->reg,&id_dest->val,4);
  printf("sltu %x %x %x\n",id_dest->val,id_src->val,id_src2->val);
}

make_EHelper(xor){
  rtl_xor(&id_dest->val,&id_src->val,&id_src2->val);
  rtl_sr(id_dest->reg,&id_dest->val,4);
  printf("xor %x %x %x\n",id_dest->val,id_src->val,id_src2->val);
}

make_EHelper(or){
  rtl_or(&id_dest->val,&id_src->val,&id_src2->val);
  rtl_sr(id_dest->reg,&id_dest->val,4);
  printf("or %x %x %x\n",id_dest->val,id_src->val,id_src2->val);
}

make_EHelper(add){
  rtl_add(&id_dest->val,&id_src->val,&id_src2->val);
  rtl_sr(id_dest->reg,&id_dest->val,4);
  printf("add %x %x %d\n",id_dest->val,id_src->val,id_src2->val);
}

make_EHelper(sub){
  rtl_sub(&id_dest->val,&id_src->val,&id_src2->val);
  rtl_sr(id_dest->reg,&id_dest->val,4);
  printf("sub %x %x %d\n",id_dest->val,id_src->val,id_src2->val);
}

make_EHelper(sri){
  printf("sri %d %d\n",decinfo.isa.instr.funct7>>5,id_src2->reg);
  if(decinfo.isa.instr.funct7>>5){
    int32_t tmp = id_src->val;
    tmp = tmp >> id_src2->reg;
    rtl_sr(id_dest->reg,&tmp,4);
  }
  else{
    uint32_t tmp = id_src->val;
    tmp = tmp >> id_src2->reg;
    rtl_sr(id_dest->reg,&tmp,4);
  }
}

make_EHelper(sll) {
  int32_t tmp = id_src->val;
  tmp = tmp << id_src2->val;
  rtl_sr(id_dest->reg,&tmp,4);
}

make_EHelper(andi){
  rtl_or(&id_dest->val,&id_src->val,&id_src2->val);
  rtl_sr(id_dest->reg,&id_dest->val,4);
}

make_EHelper(and){
  rtl_and(&id_dest->val,&id_src->val,&id_src2->val);
  rtl_sr(id_dest->reg,&id_dest->val,4);
}