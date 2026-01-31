#include <kernel/drivers/mouse.h>
#include <kernel/drivers/vga.h>
#include <lib/com1.h>
#include <mlibc/mlibc.h>

static mouse_state_t g_mouse_state = {40, 12, 0};
static u8 mouse_cycle = 0;
static s8 mouse_byte[3];

/*
 * Bitmap from USER:
 * 0 - empty, 1 - body, 2 - outline
 */
static const u8 cursor_bitmap[] = {
    2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 2, 0, 0, 0, 0, 0,
    0, 0, 2, 1, 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 2, 0, 0,
    0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1,
    2, 0, 0, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 2, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 2, 0, 2, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 1, 1,
    2, 0, 0, 0, 0, 0, 2, 1, 2, 0, 2, 1, 1, 2, 0, 0, 0, 0, 2, 2, 0, 0, 2,
    1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0};

static void mouse_wait(u8 type) {
  u32 timeout = 100000;
  if (type == 0) {
    while (timeout--) {
      if ((inb(0x64) & 1) == 1)
        return;
    }
  } else {
    while (timeout--) {
      if ((inb(0x64) & 2) == 0)
        return;
    }
  }
}

static void mouse_write(u8 val) {
  mouse_wait(1);
  outb(0x64, 0xD4);
  mouse_wait(1);
  outb(0x60, val);
}

static u8 mouse_read() {
  mouse_wait(0);
  return inb(0x60);
}

void mouse_init() {
  mouse_wait(1);
  outb(0x64, 0xA8);

  mouse_wait(1);
  outb(0x64, 0x20);
  mouse_wait(0);
  u8 status = (inb(0x60) | 2);
  mouse_wait(1);
  outb(0x64, 0x60);
  mouse_wait(1);
  outb(0x60, status);

  mouse_write(0xF6);
  mouse_read();

  mouse_write(0xF4);
  mouse_read();

  com1_printf("Mouse: Initialized successfully\n");
}

void mouse_handler() {
  u8 status = inb(0x64);
  if (!(status & 1) || !(status & 0x20))
    return;

  u8 data = inb(0x60);

  if (mouse_cycle == 0 && !(data & 0x08))
    return;

  mouse_byte[mouse_cycle++] = (s8)data;

  if (mouse_cycle == 3) {
    mouse_cycle = 0;

    int old_x = g_mouse_state.x;
    int old_y = g_mouse_state.y;

    g_mouse_state.x += mouse_byte[1] / 8;
    g_mouse_state.y -= mouse_byte[2] / 8;

    if (g_mouse_state.x < 0)
      g_mouse_state.x = 0;
    if (g_mouse_state.y < 0)
      g_mouse_state.y = 0;
    if (g_mouse_state.x >= 80)
      g_mouse_state.x = 79;
    if (g_mouse_state.y >= 25)
      g_mouse_state.y = 24;

    g_mouse_state.buttons = mouse_byte[0] & 0x07;

    if (old_x != g_mouse_state.x || old_y != g_mouse_state.y) {
      char *v = (char *)0xb8000;
      v[(old_y * 80 + old_x) * 2 + 1] ^= 0x77;
      v[(g_mouse_state.y * 80 + g_mouse_state.x) * 2 + 1] ^= 0x77;
    }
  }
}

mouse_state_t *get_mouse_state() { return &g_mouse_state; }
