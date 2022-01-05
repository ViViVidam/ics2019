#include "cpu/exec.h"

make_EHelper(ld) {
  rtl_lm(&s0, &id_src->addr, decinfo.width);
  rtl_sr(id_dest->reg, &s0, 4);
  printf("ld %x\n",s0);
  switch (decinfo.width) {
    case 4: print_asm_template2(lw); break;
    case 2: print_asm_template2(lhu); break;
    case 1: print_asm_template2(lbu); break;
    default: assert(0);
  }
}

make_EHelper(lh) {
  int32_t val = 0;
  rtl_lm(&s0,&id_src->addr, decinfo.width);
  val = s0;
  printf("lh %x\n",val);
  val = val<<16;
  val = val>>16;
  rtl_sr(id_dest->reg,&val,4);
  printf("lh %x\n",val);
}

make_EHelper(lb) {
  int32_t val = 0;
  rtl_lm(&s0,&id_src->addr, decinfo.width);
  val = s0;
  printf("lb %x\n",val);
  val = val<<24;
  val = val>>24;
  rtl_sr(id_dest->reg,&val,4);
  printf("lb %x\n",val);
}

make_EHelper(st) {
  rtl_sm(&id_src->addr, &id_dest->val, decinfo.width);
  printf("st %x\n",id_dest->val);
  switch (decinfo.width) {
    case 4: print_asm_template2(sw); break;
    case 2: print_asm_template2(sh); break;
    case 1: print_asm_template2(sb); break;
    default: assert(0);
  }
}

make_EHelper(sh) {
  rtl_sm(&id_src->addr, &id_dest->val, decinfo.width);
  printf("sm %x\n",id_dest->val);
}

make_EHelper(sb) {
  rtl_sm(&id_src->addr, &id_dest->val, decinfo.width);
  printf("sb %x\n",id_dest->val);
}