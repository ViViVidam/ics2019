#include "cpu/exec.h"
make_EHelper(auipc) {
  int32_t tmp = cpu.pc + id_src;
  printf("auipc cpu:%x %d\n",cpu.pc,id_dest->reg);
  rtl_sr(id_dest->reg,&tmp,4);
}

make_EHelper(jal){
	cpu.pc += id_src;
}