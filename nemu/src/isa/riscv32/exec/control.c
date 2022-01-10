#include "cpu/exec.h"
make_EHelper(jal){
  uint32_t nextpc = cpu.pc + 4;
  rtl_sr(id_dest->reg,&(nextpc),4);
	cpu.pc += id_src->val;
	decinfo.jmp_pc = cpu.pc;
  //printf("jal pc:%x\n",cpu.pc);
}

make_EHelper(jalr){
  uint32_t nextpc = cpu.pc + 4;
  int32_t tmp = id_src->val + id_src2->val;
  tmp = tmp >> 1;
  tmp = tmp << 1;
  rtl_sr(id_dest->reg,&(nextpc),4);
  cpu.pc = tmp;
  //printf("jalr pc:%x %x %x\n",cpu.pc,id_src->val,id_src2->val);
}

make_EHelper(beq){
  rtl_jrelop(RELOP_EQ,&id_src->val,&id_src2->val,decinfo.jmp_pc);
  print_asm_template2(beq);
}

make_EHelper(bne){
  rtl_jrelop(RELOP_NE,&id_src->val,&id_src2->val,decinfo.jmp_pc);
  print_asm_template2(bne);
}

make_EHelper(blt){
  rtl_jrelop(RELOP_LT,&id_src->val,&id_src2->val,decinfo.jmp_pc);
  print_asm_template2(blt);
}

make_EHelper(bge){
  rtl_jrelop(RELOP_GE,&id_src->val,&id_src2->val,decinfo.jmp_pc);
  print_asm_template2(bge);
}

make_EHelper(bltu){
  rtl_jrelop(RELOP_LTU,&id_src->val,&id_src2->val,decinfo.jmp_pc);
  print_asm_template2(bltu);
}

make_EHelper(bgeu){
  rtl_jrelop(RELOP_GEU,&id_src->val,&id_src2->val,decinfo.jmp_pc);
  print_asm_template2(bgeu);
}