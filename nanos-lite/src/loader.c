#include "proc.h"
#include <elf.h>
#include <fs.h>
extern uint8_t ramdisk_start;
#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr header;
  Elf_Phdr segment;
  Elf32_Addr vaddr;
  Elf32_Word filesize;
  Elf32_Word memsize;
  Elf32_Off segmentoffset = 0;
  Elf32_Off segcontent_off = 0;
  ramdisk_read(&header, 0, sizeof(Elf32_Ehdr));
  printf("num %d\n",header.e_phnum);
  segmentoffset = header.e_phoff;
  for(int i=0;i<header.e_phnum;i++){
    ramdisk_read(&segment,header.e_phoff,sizeof(segment));
    if(segment.p_type==PT_LOAD){
      printf("pt load\n");
    }
    ramdisk_write(segment.p_vaddr,header.e_ehsize+header.e_phnum*header.e_phentsize+segment.p_offset,segment.p_filesz);
    printf("%d %d",segment.p_offset,segment.p_memsz);
    memset(segment.p_vaddr+segment.p_filesz,0,segment.p_memsz-segment.p_filesz);
  }
  return header.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  printf("naive load\n");
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  if(entry%4!=0){
    printf("invalid addr\n");
  }
  ((void(*)())entry) ();
}
//finished loader
/*
static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr head;
  int fd=fs_open(filename,0,0);
  fs_lseek(fd,0,SEEK_SET);
  fs_read(fd,&head,sizeof(head));
  for(int i=0;i<head.e_phnum;i++){
    Elf_Phdr temp;
    fs_lseek(fd,head.e_phoff+i*head.e_phentsize,SEEK_SET);
    fs_read(fd,&temp,sizeof(temp));
    if(temp.p_type==PT_LOAD){
      fs_lseek(fd,temp.p_offset,SEEK_SET);
      fs_read(fd,(void*)temp.p_vaddr,temp.p_filesz);
      memset((void*)(temp.p_vaddr+temp.p_filesz),0,temp.p_memsz-temp.p_filesz);
    }
  }
  return head.e_entry;
}*/

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
