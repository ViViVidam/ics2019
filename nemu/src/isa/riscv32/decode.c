#include "cpu/decode.h"
#include "rtl/rtl.h"

// decode operand helper
#define make_DopHelper(name) void concat(decode_op_, name) (Operand *op, uint32_t val, bool load_val)

//是否要读取地址的值
static inline make_DopHelper(i) {
  op->type = OP_TYPE_IMM;
  op->imm = val;
  if(load_val){
    rtl_li(&op->val, op->imm);
  }

  print_Dop(op->str, OP_STR_SIZE, "%d", op->imm);
}

//是否要从寄存器中读取值
static inline make_DopHelper(r) {
  op->type = OP_TYPE_REG;
  op->reg = val;
  if (load_val) {
    rtl_lr(&op->val, op->reg, 4);
  }

  print_Dop(op->str, OP_STR_SIZE, "%s", reg_name(op->reg, 4));
}

make_DHelper(U) {
  decode_op_i(id_src, decinfo.isa.instr.imm31_12 << 12, true);
  decode_op_r(id_dest, decinfo.isa.instr.rd, false);

  print_Dop(id_src->str, OP_STR_SIZE, "0x%x", decinfo.isa.instr.imm31_12);
}

//每个decode helper的任务就是填满src,src2以及dest
make_DHelper(ld) {
  decode_op_r(id_src, decinfo.isa.instr.rs1, true);
  decode_op_i(id_src2, decinfo.isa.instr.simm11_0, true);

  print_Dop(id_src->str, OP_STR_SIZE, "%d(%s)", id_src2->val, reg_name(id_src->reg, 4));

  rtl_add(&id_src->addr, &id_src->val, &id_src2->val);

  decode_op_r(id_dest, decinfo.isa.instr.rd, false);
}

make_DHelper(st) {
  //rs1+simm = address
  //*address -> rd
  decode_op_r(id_src, decinfo.isa.instr.rs1, true);//rs1
  int32_t simm = (decinfo.isa.instr.simm11_5 << 5) | decinfo.isa.instr.imm4_0;
  decode_op_i(id_src2, simm, true);

  print_Dop(id_src->str, OP_STR_SIZE, "%d(%s)", id_src2->val, reg_name(id_src->reg, 4));

  rtl_add(&id_src->addr, &id_src->val, &id_src2->val);

  decode_op_r(id_dest, decinfo.isa.instr.rs2, true);
}
make_DHelper(addi){
  decode_op_r(id_src,decinfo.isa.instr.rs1,true);
  decode_op_i(id_src2,decinfo.isa.instr.simm11_0,false);

  decode_op_r(id_dest,decinfo.isa.instr.rd,false);
  //print_Dop(id_src->str,OP_STR_SIZE,"0x%x",defin)
}