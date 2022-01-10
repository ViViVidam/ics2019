#include "cpu/exec.h"
make_EHelper(div){
  uint32_t val1 = id_src->val;
  uint32_t val2 = id_src2->val;
  //printf("%d\n",val1/val2);
  rtl_div_q(&id_dest->val,&val1,&val2);
  rtl_sr(id_dest->reg,&id_dest->val,4);
}
