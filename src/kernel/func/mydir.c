#include <kernel/drivers/vga.h>
#include <kernel/func/mydir.h>

void mydir(char *dir, int *current_line) {
  print("Your dir: ", (*current_line)++, 0, 0x0F);
  print(dir, *current_line - 1, 11, 0x0F);
  print(dir, *current_line, 0, 0x0F);
  print(">", *current_line, 1, 0x0F);
}