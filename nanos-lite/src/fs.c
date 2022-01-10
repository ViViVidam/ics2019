#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

extern size_t ramdisk_read(void*, size_t, size_t);
extern size_t ramdisk_write(const void*, size_t, size_t);
size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
size_t fbsync_write(const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, invalid_read, serial_write},
  {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
  {"/dev/events", 0, 0, 0, events_read, invalid_write},
  {"/dev/fbsync", 0, 0, 0, invalid_read,fbsync_write},
  {"/proc/dispinfo", 128, 0, 0, dispinfo_read,invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

int fs_open(const char *pathname, int flags, int mode){
  printf("%s\n",pathname);
  for(int i=3;i<NR_FILES;i++){
     printf("%s\n",file_table[i].name);
    if(strcmp(pathname,file_table[i].name)==0){
      //printf("filename %d\n");
      return i;
    }
  }
  printf("file open failed\n");
  assert(0);
  return 0;
}
size_t fs_read(int fd, void *buf, size_t len){
  if(file_table[fd].open_offset+len>=file_table[fd].size)
    len=file_table[fd].size-file_table[fd].open_offset;
  if(len<0)
    len=0;
  if(file_table[fd].read==NULL)
    ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);
  else
    len=file_table[fd].read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);
  file_table[fd].open_offset+=len;
  return len;
}
int fs_close(int fd){
  file_table[fd].open_offset=0;
  return 0;
}
size_t fs_write(int fd, const void *buf, size_t len){
  size_t length=0;
  if(fd==1||fd==2){
    serial_write("before\n",0,34);
  }
  if(file_table[fd].open_offset+len>=file_table[fd].size)
    len=file_table[fd].size-file_table[fd].open_offset;
  if(fd==1||fd==2){
    serial_write("after\n",0,34);
  }
  if(file_table[fd].write==NULL||file_table[fd].write!=&invalid_write)
    length=ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
  else{
    if(fd==1)
      serial_write("111111\n",0,32);
    else if(fd==2)
      serial_write("22222\n",0,32);
    length=file_table[fd].write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
  }
    file_table[fd].open_offset+=length;
  return length;
}

size_t fs_lseek(int fd, size_t offset, int whence){
  switch(whence){
        case SEEK_SET:
            file_table[fd].open_offset=offset;
            break;
        case SEEK_CUR:
            file_table[fd].open_offset+=offset;
            break;
        case SEEK_END:
            file_table[fd].open_offset=file_table[fd].size+offset;
            break;
    }
    return file_table[fd].open_offset;
}

void init_fs() {
  file_table[FD_FB].size=screen_width()*screen_height()*4;
}