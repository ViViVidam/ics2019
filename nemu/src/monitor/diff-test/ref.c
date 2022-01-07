#include "nemu.h"
#include "monitor/diff-test.h"
#include "isa/diff-test.h"

void cpu_exec(uint64_t);
// 从DUT host memory的`src`处拷贝`n`字节到REF guest memory的`dest`处
void difftest_memcpy_from_dut(paddr_t dest, void *src, size_t n) {
  memcpy(guest_to_host(dest), src, n);
}
// 获取REF的寄存器状态到`r`
void difftest_getregs(void *r) {
  memcpy(r, &cpu, DIFFTEST_REG_SIZE);
}
// 设置REF的寄存器状态为`r
void difftest_setregs(const void *r) {
  memcpy(&cpu, r, DIFFTEST_REG_SIZE);
}
// 让REF执行`n`条指令
void difftest_exec(uint64_t n) {
  cpu_exec(n);
}
// 初始化REF的DiffTest功能
void difftest_init(void) {
  /* Perform ISA dependent initialization. */
  void init_isa();
  init_isa();
}
