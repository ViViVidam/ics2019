#include <am.h>
#include <amdev.h>
#include <nemu.h>

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      uint32_t inf = 0;
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      info = inl(SCREEN_ADDR);
      info->width = inf>>16;
      info->height = inf&65535;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}
//int x, y; uint32_t *pixels; int w, h, sync
size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;

      if (ctl->sync) {
        outl(SYNC_ADDR, 0);
      }
      printf("done\n");
      return size;
    }
  }
  return 0;
}

void __am_vga_init() {
  int i;
  int size = screen_width() * screen_height();
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < size; i ++) fb[i] = i;
  printf("done1\n");
  draw_sync();
}