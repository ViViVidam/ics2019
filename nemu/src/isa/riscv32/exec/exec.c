#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry load_table [8] = {
  EMPTY, EMPTY, EXW(ld, 4), EMPTY, EXW(lbu,1), EMPTY, EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EXW(sb,1), EXW(sh,2), EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry immediate_table [8] = {
  EXW(addi,4),EMPTY,EMPTY,EXW(sltiu,4),EMPTY,IDEX(shift,sri),EMPTY,EMPTY
};

static make_EHelper(immediate){
  decinfo.width = immediate_table[decinfo.isa.instr.funct3].width;
  idex(pc, &immediate_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry op_table [2][8]={
  {EXW(add,4),EMPTY,EMPTY,EXW(sltu,4),EXW(xor,4),EMPTY,EXW(or,4),EMPTY},
  {EXW(sub,4),EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY}
};
static make_EHelper(operation){
  decinfo.width = op_table[decinfo.isa.instr.funct7>>5][decinfo.isa.instr.funct3].width;
  idex(pc, &op_table[decinfo.isa.instr.funct7>>5][decinfo.isa.instr.funct3]);
}

static OpcodeEntry branch_table[8]={
  EXW(beq,4),EXW(bne,4),EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY
};

static make_EHelper(branch){
  decinfo.width = branch_table[decinfo.isa.instr.funct3].width;
  idex(pc, &branch_table[decinfo.isa.instr.funct3]);
}



static OpcodeEntry opcode_table [32] = {
  /* b00 */ IDEX(ld, load), EMPTY, EMPTY, EMPTY, IDEX(immediate,immediate), IDEX(auipc,auipc), EMPTY, IDEX(lui,lui),
  /* b01 */ IDEX(st, store), EMPTY, EMPTY, EMPTY, IDEX(op,operation), IDEX(U, lui), EMPTY, EMPTY,
  /* b10 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b11 */ IDEX(branch,branch), IDEX(jalr,jalr), EX(nemu_trap), IDEX(jal,jal), EMPTY, EMPTY, EMPTY, EMPTY,
};

void  isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);//val就是得到的地址值
  assert(decinfo.isa.instr.opcode1_0 == 0x3);
  printf("\n%x opcode: %d\n",cpu.pc,decinfo.isa.instr.opcode6_2);
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
