#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t i = 0;
  while(s[i]!='\0'){
    printf("1\n");
    i++;
  }
  return i;
}

char *strcpy(char* dst,const char* src) {
  //biggest type to held any data
  size_t pointer = 0;
  while(*(src+pointer)!='\0'){
    *(dst+pointer) = *(src+pointer);
    pointer++;
  }
  *(dst+pointer)='\0';
  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  size_t i;
  for (i = 0; i < n && src[i] != '\0'; i++)
     dst[i] = src[i];
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
  for(i=0;s1[i]!='\0';i++){
    if(s2[i]=='\0'){
      return 1;
    }
    tmp = s1[i]-s2[i];
    if(tmp>0){
      return 1;
    }
    else if(tmp<0){
      return -1;
    }
  }
  if(s2[i]!='\0')
    return -1;
  else
    return 0;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  size_t i = 0;
  uint8_t tmp = 0;
  for(i=0;i<n&&s1[i]!='\0';i++){
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
  char* p=v;
  while(n--)
  {
      *p++ = (unsigned char)c;
  }
  return v;
}

void* memcpy(void* out, const void* in, size_t n) {
  size_t i = 0;
  char* p = out;
  char* t = in;
  for(i=0;i<n;i++){
    p[i] = t[i];
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n){
  char* p = s1;
  char* t = s2;
  char tmp = 0;
  while(n--){
    tmp = *p-*t;
    if(tmp>0){
      return 1;
    }
    else if(tmp<0){
      return -1;
    }
  }
  return 0;
}

#endif
