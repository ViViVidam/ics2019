#include "proc.h"
#include <elf.h>

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
  Elf32_Word memsize;
  Elf32_Off segmentoffset = 0;
  Elf32_Off segcontent_off = 0;
  ramdisk_read(&header, 0, sizeof(Elf32_Ehdr));
  segcontent_off = header.e_phoff;
  ramdisk_read(&segment, segmentoffset, sizeof(Elf32_Ehdr));
  segcontent_off = segment.p_offset;
  memsize = segment.p_memsz;
  vaddr = segment.p_vaddr;
  memset(vaddr,0,memsize);
  memcpy(vaddr,&ramdisk_start+segcontent_off+segmentoffset,memsize);
  return header.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
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
