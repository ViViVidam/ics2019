#include <am.h>
#include <amdev.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

size_t __am_input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      uint32_t tmp = 0;
      _DEV_INPUT_KBD_t *kbd = (_DEV_INPUT_KBD_t *)buf;
      tmp = inl(KBD_ADDR);
      kbd->keydown = tmp>>31;
      kbd->keycode = tmp&0x7FFF;
      
        kbd->keydown=!kbd->keydown;
        //printf("%d\n",tmp)
      return sizeof(_DEV_INPUT_KBD_t);
    }
  }
  return 0;
}
