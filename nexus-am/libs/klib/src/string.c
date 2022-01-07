#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t i = 0;
  while(s[i]!='\0'){
    i++;
  }
  return i;
}

char *strcpy(char* dst,const char* src) {
  //biggest type to held any data
  size_t pointer = 0;
  while(*(src+pointer)){
    *(dst+pointer) = *(src+pointer);
  }
  *(dst+pointer)='\0';
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  size_t i;
  for (i = 0; i < n && src[i] != '\0'; i++)
     dst[i] = src[i];
  for ( ; i < n; i++)
     dst[i] = '\0';

  return dst;
}

char* strcat(char* dst, const char* src) {
  size_t pointer = 0;
  while(*(dst+pointer)!='\0'){
    pointer++;
  }
  while(*src!='\0'){
    *(dst+pointer) = *src;
    src++;
    pointer++;
  }
  *(dst+pointer)='\0';
  return dst;
}

int strcmp(const char* s1, const char* s2) {
  size_t i = 0;
  uint8_t tmp = 0;
  while(*(s1+i)!='\0'){
    if(*(s2+i)==0){
      return 1;
    }
    tmp = *(s1+i)-*(s2+i);
    if(tmp>0){
      return 1;
    }
    else if(tmp<0){
      return -1;
    }
  }
  if(*(s2+i)!='\0')
    return -1;
  else
    return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  size_t i = 0;
  uint8_t tmp = 0;
  for(i=0;i<n;i++){

    if(*(s2+i)==0){
      return 1;
    }
    tmp = *(s1+i)-*(s2+i);
    if(tmp>0){
      return 1;
    }
    else if(tmp<0){
      return -1;
    }
  }
  if(*(s2+i)!='\0')
    return -1;
  else
    return 0;
}

void* memset(void* v,int c,size_t n) {
  /*size_t i = 0;
  for(i=0;i<n;i++){
    isa_vaddr_write(v, c, 1);
  }*/
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {

  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n){
  return 0;
}

#endif
