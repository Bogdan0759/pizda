#include <kernel/bin/shell.h>
#include <kernel/drivers/disk/pata/pata.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/drivers/vga.h>
#include <kernel/interrupts/idt.h>
#include <lib/com1.h>
#include <mlibc/mlibc.h>

// Точка входа(если что-то сломалось то здесь)
void kmain() {
  init_idt();
  init_heap();
  pata_identify();
  clear_screen();

  print("OTSOS started!", 0, 30, 0x0A);

  com1_init();

  com1_write_string("OTSOS started at address ");
  com1_write_hex_qword((u64)kmain);
  com1_newline();

  print("There's nothing here yet =)", 3, 0, 0x0F);

  char s_buf[2048];
  int current_line = 5;
  char dir[256] = "/"; // Потом когда добавим файловую систему нужнл будет немного изменить

  print(dir, current_line, 0, 0x0F);
  print(">", current_line, 1, 0x0F);

  while (1) {
    scanf(s_buf, 2048, current_line, 2, 0x0F, dir);
    shell_func(s_buf, &current_line, dir);

    if (current_line >= 25) {
      current_line = 24;
    }
  }
}