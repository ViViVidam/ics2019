#include "cpu/exec.h"
make_EHelper(jal){
	cpu.pc += id_src->val;
	decinfo.jmp_pc = cpu.pc;
  printf("%d\n",id_dest->reg);
  rtl_sr(&id_dest->reg,&(cpu.pc),4);
  printf("jal pc:%x\n",cpu.pc);
}

make_EHelper(jalr){
  int32_t tmp = id_src->val + id_src2->val;
  tmp = tmp >> 1;
  tmp = tmp << 1;
  rtl_sr(&id_dest->reg,&(cpu.pc),4);
  cpu.pc = tmp;
  printf("jalr pc:%x\n",cpu.pc);
}