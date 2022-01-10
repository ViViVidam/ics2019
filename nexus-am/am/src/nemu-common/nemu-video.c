#include <am.h>
#include <amdev.h>
#include <nemu.h>


void draw_sync();
int screen_width();
int screen_height();

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      uint32_t inf = 0;
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      //inf = inl(SCREEN_ADDR);
      info->width = screen_width();
      info->height = screen_height();
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}
//int x, y; uint32_t *pixels; int w, h, sync
size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      int i = 0,j = 0;
      int W = screen_width(), H = screen_height();
      uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;
      for(i=0;i<ctl->w;i++){
        for(j=0;j<ctl->h;j++){
          fb[(ctl->y+j)*W+ctl->x+i]=*(ctl->pixels+j*(ctl->w)+i);
        }
      }
      if (ctl->sync) {
        outl(SYNC_ADDR, 0);
      }
      //printf("(%d,%d) (%d,%d)\n",ctl->x,ctl->y,ctl->w,ctl->h);
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
  //printf("done1\n");
  draw_sync();
}