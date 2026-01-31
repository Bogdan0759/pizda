#ifndef MOUSE_H
#define MOUSE_H

#include <mlibc/mlibc.h>

typedef struct {
  int x;
  int y;
  u8 buttons;
} mouse_state_t;

void mouse_init();
void mouse_handler();
mouse_state_t *get_mouse_state();

#endif
