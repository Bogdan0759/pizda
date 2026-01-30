#include <kernel/bin/mydir.h>
#include <kernel/drivers/vga.h>
#include <kernel/drivers/fs/chainFS/chainfs.h>
#include <mlibc/mlibc.h>

void mydir(char *dir, int *current_line) {
  char current_path[256];
  if (chainfs_get_current_path(current_path, 256)) {
    print("Current directory: ", (*current_line)++, 0, 0x0F);
    print(current_path, (*current_line)++, 0, 0x0A);
    strcpy(dir, current_path);
  } else {
    print("Error getting current directory", (*current_line)++, 0, 0x0C);
  }
  
  print(dir, *current_line, 0, 0x0F);
  print(">", *current_line, 1, 0x0F);

  if (*current_line >= 25) {
    *current_line = 24;
  }
}