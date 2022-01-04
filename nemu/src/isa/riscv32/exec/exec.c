#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry load_table [8] = {
  EMPTY, EMPTY, EXW(ld, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EMPTY, EMPTY, EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry immediate_table [8] = {
  EXW(addi,4),EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY
};

static make_EHelper(add_immediate){
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  printf("funct3 %d\n",decinfo.isa.instr.funct3);
  idex(pc, &immediate_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry opcode_table [32] = {
  /* b00 */ IDEX(ld, load), EMPTY, EMPTY, IDEX(jal,jal), IDEX(addi,add_immediate), IDEX(auipc,auipc), EMPTY, EMPTY,
  /* b01 */ IDEX(st, store), EMPTY, EMPTY, EMPTY, EMPTY, IDEX(U, lui), EMPTY, EMPTY,
  /* b10 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b11 */ EMPTY, EMPTY, EX(nemu_trap), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
};

void  isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);//val就是得到的地址值
  assert(decinfo.isa.instr.opcode1_0 == 0x3);
  printf("\nopcode: %d\n",decinfo.isa.instr.opcode6_2);
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
