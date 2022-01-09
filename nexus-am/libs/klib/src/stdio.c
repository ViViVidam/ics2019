#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
char stack[100];
int stacktop=0;

int printf(const char *fmt, ...) {
  va_list args;
  _putc("1");
  _putc("\n");
  int num = 0;
  int tmp = 0;
  unsigned int tmp2 = 0;
  int i = 0;
  char mod = 0;
  char* string;
  char c;
  va_start(args,fmt);
  while(*fmt){
    if(*fmt=='%'){
      fmt++;
      switch(*fmt){
      case 'd':
      num = va_arg(args,int);
      tmp = num;
      if(tmp<0){
        _putc('-');
        tmp = -tmp;
      }
      else if(tmp==0){
        _putc('0');
      }
      while(tmp){
        mod = (char) tmp%10;
        tmp = tmp/10;
        stack[stacktop++]='0'+mod;
      }
      while(stacktop){
        stacktop--;
        _putc(stack[stacktop]);
      }
      break;
    case 'x':
      _putc('0');
      _putc('x');
      num = va_arg(args,int);
      tmp2 = num;
      while(tmp2){
        mod = (char) tmp2%16;
        tmp2 = tmp2/16;
        if(mod>9)
          stack[stacktop++]='A'+mod-10;
        else
          stack[stacktop++]='0'+mod;
      }
      while(stacktop){
        stacktop--;
        _putc(stack[stacktop]);
      }
      break;
    case 'c':
      c = (char) va_arg(args,int);
      _putc(c);
      break;
    case 's':
      string = va_arg(args,char*);
      while(*string){
        _putc(*string);
        string++;
      }
      }
    }
    else{
      _putc(*fmt);
    }
    fmt++;
  }
  va_end(args);
  return i;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  int num = 0;
  int tmp = 0;
  int i = 0;
  char mod = 0;
  char stack[100];
  int stacktop=0;
  char* string;
  char c;
  va_start(args,fmt);
  while(*fmt){
    if(*fmt=='%'){
      fmt++;
      switch(*fmt){
      case 'd':
      num = va_arg(args,int);
      tmp = num;
      if(tmp<0){
        out[i++]='-';
        tmp = -tmp;
      }
      else if(tmp==0){
        out[i++]='0';
      }
      while(tmp){
        mod = (char) tmp%10;
        tmp = tmp/10;
        stack[stacktop++]='0'+mod;
      }
      while(stacktop){
        stacktop--;
        out[i++] = stack[stacktop];
      }
      break;
    case 'c':
      c = (char) va_arg(args,int);
      out[i++] = c;
      break;
    case 's':
      string = va_arg(args,char*);
      while(*string){
        out[i++]=*string;
        string++;
      }
      }
    }
    else{
      out[i++]=*fmt;
    }
  fmt++;
  }
  va_end(args);
  out[i]=0;
  return i;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
