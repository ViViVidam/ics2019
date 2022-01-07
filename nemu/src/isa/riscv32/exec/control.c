#include "cpu/exec.h"
make_EHelper(jal){
  uint32_t nextpc = cpu.pc + 4;
  rtl_sr(id_dest->reg,&(nextpc),4);
	cpu.pc += id_src->val;
	decinfo.jmp_pc = cpu.pc;
  printf("jal pc:%x\n",cpu.pc);
}

make_EHelper(jalr){
  uint32_t nextpc = cpu.pc + 4;
  int32_t tmp = id_src->val + id_src2->val;
  tmp = tmp >> 1;
  tmp = tmp << 1;
  rtl_sr(id_dest->reg,&(nextpc),4);
  cpu.pc = tmp;
  printf("jalr pc:%x %x %x\n",cpu.pc,id_src->val,id_src2->val);
}

make_EHelper(beq){
  if(id_src->val == id_src2->val){
    cpu.pc += id_dest->val;
    decinfo.is_jmp = true;
    printf("beq pc:%x\n",cpu.pc);
  }
  else
    printf("beq no jump"); 
}

make_EHelper(bne){
  if(id_src->val != id_src2->val){
    cpu.pc += id_dest->val;
    decinfo.is_jmp = true;
    printf("bne pc:%x\n",cpu.pc);
  }
  else
    printf("bne no jump"); 
}

make_EHelper(blt){
  int32_t val1 = id_src->val;
  int32_t val2 = id_src2->val;
  if(val1 < val2){
    cpu.pc += id_dest->val;
    decinfo.is_jmp = true;
    printf("blt pc:%x\n",cpu.pc);
  }
  else
    printf("blt no jump"); 
}

make_EHelper(bge){
  int32_t val1 = id_src->val;
  int32_t val2 = id_src2->val;
  if(val1 > val2 || val1 == val2){
    cpu.pc += id_dest->val;
    decinfo.is_jmp = true;
    printf("bge pc:%x\n",cpu.pc);
  }
  else
    printf("bge no jump"); 
}