#include "cpu/exec.h"
make_EHelper(jal){
	cpu.pc += id_src->val;
	decinfo.jmp_pc = cpu.pc;
  printf("jal pc:%x\n",cpu.pc);
}