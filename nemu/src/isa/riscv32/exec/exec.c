#include "cpu/exec.h"
#include "all-instr.h"

static OpcodeEntry load_table [8] = {
  EXW(lb,1), EXW(lh,2), EXW(ld, 4), EMPTY, EXW(ld,1), EXW(ld,2), EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry store_table [8] = {
  EXW(st,1), EXW(st,2), EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry immediate_table [8] = {
  EXW(addi,4),IDEX(shift,slli),EMPTY,EXW(sltiu,4),EXW(xori,4),IDEX(shift,sri),EMPTY,EXW(andi,4)
};

static make_EHelper(immediate){
  decinfo.width = immediate_table[decinfo.isa.instr.funct3].width;
  idex(pc, &immediate_table[decinfo.isa.instr.funct3]);
}

/* for add sub and mul */
static OpcodeEntry extended1 [5]={
  EXW(add,4),EXW(sub,4),EXW(mul,4),EXW(xor,4),EXW(div,4)
};

static make_EHelper(add_sub_mul){
  printf("add_sub_mul %d\n",decinfo.isa.instr.funct7);
  if(decinfo.isa.instr.funct7==0){
    decinfo.width = extended1[0].width;
    idex(pc,&extended1[0]);
  }
  else if(decinfo.isa.instr.funct7==1){
    decinfo.width = extended1[1].width;
    idex(pc,&extended1[1]);
  }
  else{
    decinfo.width = extended1[2].width;
    idex(pc,&extended1[2]);
  }
}

static make_EHelper(xor_div){
  if(decinfo.isa.instr.funct7==0){
    decinfo.width = extended1[3].width;
    idex(pc,&extended1[3]);
  }
  else{
    decinfo.width = extended1[4].width;
    idex(pc,&extended1[4]);
  }
}

static OpcodeEntry op_table [8]={
  EXW(add_sub_mul,4),EXW(sll,4),EXW(slt,4),EXW(sltu,4),EXW(xor_div,4),EMPTY,EXW(or,4),EXW(and,4)
};
static make_EHelper(operation){
  decinfo.width = op_table[decinfo.isa.instr.funct3].width;
  idex(pc, &op_table[decinfo.isa.instr.funct3]);
}

static OpcodeEntry branch_table[8]={
  EXW(beq,4),EXW(bne,4),EMPTY,EMPTY,EXW(blt,4),EXW(bge,4),EMPTY,EMPTY
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
