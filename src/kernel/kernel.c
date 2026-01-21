#include <kernel/drivers/disk/pata/pata.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/drivers/vga.h>
#include <kernel/func/shell.h>
#include <kernel/interrupts/idt.h>
#include <lib/com1.h>
#include <mlibc/mlibc.h>

// Точка входа(если что-то сломалось то здесь)
void kmain() {
  init_idt();
  init_heap();
  pata_identify();

  // ТЕСТ ЗАПИСИ И ЧТЕНИЯ ДИСКА (DISK TEST)
  com1_printf("Testing Disk R/W...\n");
  unsigned char *disk_buf = (unsigned char *)kmalloc(512);
  if (disk_buf) {
    // Подготовим данные
    for (int i = 0; i < 512; i++)
      disk_buf[i] = 0;
    char *msg = "DISK_OK: SYSTEM_READY";
    for (int i = 0; msg[i] != '\0'; i++)
      disk_buf[i] = msg[i];

    com1_printf("Writing to Sector 0...\n");
    pata_write_sector(0, disk_buf);

    com1_printf("Clearing buffer and reading back...\n");
    for (int i = 0; i < 512; i++)
      disk_buf[i] = 0;
    pata_read_sector(0, disk_buf);

    com1_printf("Sector 0 content: %s\n", (char *)disk_buf);
    kfree(disk_buf);
  }

  clear_screen();

  print("OTSOS started!", 0, 30, 0x0A);

  com1_init();

  com1_write_string("OTSOS started at address ");
  com1_write_hex_qword((u64)kmain);
  com1_newline();

  print("There's nothing here yet =)", 3, 0, 0x0F);

  char s_buf[2048];
  int current_line = 5;
  char *dir =
      "/"; // Потом когда добавим файловую систему нужнл будет немного изменить

  print(dir, current_line, 0, 0x0F);
  print(">", current_line, 1, 0x0F);

  while (1) {
    scanf(s_buf, 2048, current_line, 2, 0x0F, dir);

    shell_func(s_buf, &current_line, dir);
  }
}