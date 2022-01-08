#include "common.h"

static _Context* do_event(_Event e, _Context* c) {
  switch (e.event) {
    case 1: printf("event ID = 1\n");
    break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
