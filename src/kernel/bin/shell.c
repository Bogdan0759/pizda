#include <kernel/bin/calc.h>
#include <kernel/bin/info.h>
#include <kernel/bin/mydir.h>
#include <kernel/bin/shell.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/drivers/vga.h>
#include <mlibc/mlibc.h>

void shell_func(char *s_buf, int *current_line, char *dir) {
  (*current_line)++;
  char vendor[13];
  char model[49];

  if (strcmp(s_buf, "shutdown") == 0) {
    outb(0x64, 0xFE);
    while (1)
      ;
  } else if (strcmp(s_buf, "reboot") == 0) {
    outb(0x64, 0xFE);
  } else if (strcmp(s_buf, "mydir") == 0) {
    mydir(dir, current_line);
  } else if (strcmp(s_buf, "info") == 0) {
    get_cpu_vendor(vendor);
    get_cpu_model(model);
    print(vendor, (*current_line)++, 0, 0x0F);
    print(model, (*current_line)++, 0, 0x0F);
    print(dir, *current_line, 0, 0x0F);
    print(">", *current_line, 1, 0x0F);
  } else if (strcmp(s_buf, "clear") == 0) {
    clear_screen();
    *current_line = 1;
    print(dir, *current_line, 0, 0x0F);
    print(">", *current_line, 1, 0x0F);
  } else if (strcmp(s_buf, "help") == 0) {
    print("Command: shutdown ,reboot, clear, echo, mydir, help, clac ,info",
          (*current_line)++, 0, 0x0A);
    print(dir, *current_line, 0, 0x0F);
    print(">", *current_line, 1, 0x0F);
  } else if (s_buf[0] == 'e' && s_buf[1] == 'c' && s_buf[2] == 'h' &&
             s_buf[3] == 'o' && s_buf[4] == ' ') {
    print(&s_buf[5], *current_line, 2, 0x0F);
    (*current_line)++;
    print(dir, *current_line, 0, 0x0F);
    print(">", *current_line, 1, 0x0F);
  } else if (strcmp(s_buf, "calc") == 0) {
    calc(s_buf, current_line, dir);
  }

  else {
    print("Unknown command", *current_line, 0, 0x0F);
    (*current_line)++;
    print(dir, *current_line, 0, 0x0F);
    print(">", *current_line, 1, 0x0F);
  }
}