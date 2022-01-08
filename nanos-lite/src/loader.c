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
  /*Elf_Ehdr header;
  Elf_Phdr segment;
  Elf32_Addr vaddr;
  Elf32_Word filesize;
  Elf32_Word memsize;
  Elf32_Off segmentoffset = 0;
  Elf32_Off segcontent_off = 0;
  ramdisk_read(&header, 0, sizeof(Elf32_Ehdr));
  segmentoffset = header.e_phoff;
  for(int i=0;i<header.e_phnum;i++){
    ramdisk_read(&segment,header.e_phoff+i*header.e_phentsize,header.e_phentsize);
    if(segment.p_type==PT_LOAD){
      memcpy(segment.p_vaddr,&ramdisk_start+segment.p_offset,segment.p_filesz);
      //memset(segment.p_vaddr+segment.p_filesz,0,segment.p_memsz-segment.p_filesz);
    }
  }
  printf("%x\n",header.e_entry);
  return header.e_entry;*/
  retrun NULL;
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
