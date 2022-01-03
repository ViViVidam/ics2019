#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char expr[32];
  int val_pre;
  bool valid;
  /* TODO: Add more members if necessary */


} WP;

WP* gethead();

#endif
