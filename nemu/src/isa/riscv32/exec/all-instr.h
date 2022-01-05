#include "cpu/exec.h"

make_EHelper(lui);

make_EHelper(ld);
make_EHelper(st);
make_EHelper(sh);
make_EHelper(sb);

make_EHelper(inv);
make_EHelper(nemu_trap);

make_EHelper(addi);
make_EHelper(sltiu);

make_EHelper(sri);
make_EHelper(add);
make_EHelper(sub);
make_EHelper(sltu);
make_EHelper(xor);
make_EHelper(or);

make_EHelper(auipc);

make_EHelper(jal);
make_EHelper(jalr);
make_EHelper(beq);
make_EHelper(bne);