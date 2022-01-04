#include "cpu/exec.h"
make_EHelper(auipc) {
  int32_t tmp = cpu.pc + id_src;
  printf("auipc cpu:%x %d\n",cpu.pc,id_dest->reg);
  rtl_sr(id_dest->reg,&tmp,4);
}

make_EHelper(jal){
  vaddr_t offset = 4;
	cpu.pc += id_src->val - offset;
	decinfo.jmp_pc = cpu.pc;
  printf("jal pc:%x\n",cpu.pc);
}