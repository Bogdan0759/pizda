#include <kernel/bin/touch.h>
#include <kernel/drivers/fs/chainFS/chainfs.h>
#include <kernel/drivers/vga.h>
#include <mlibc/mlibc.h>

void touch(char *s_buf, int *current_line, char *dir) {
  if (s_buf[0] == 't' && s_buf[1] == 'o' && s_buf[2] == 'u' &&
      s_buf[3] == 'c' && s_buf[4] == 'h' && s_buf[5] == ' ') {
    char *filename = &s_buf[6];
    char *content = "";
    int content_len = 0;

    char *ptr = filename;
    while (*ptr) {
      if (ptr[0] == ' ' && ptr[1] == '-' && ptr[2] == 'C' && ptr[3] == ' ') {
        *ptr = 0;
        content = ptr + 4;
        content_len = strlen(content);
        break;
      }
      ptr++;
    }

    if (chainfs_write_file(filename, (const u8 *)content, (u32)content_len) ==
        0) {
    } else {
      print("failed to create", (*current_line)++, 0, 0x0C);
    }
  } else {
    print("Usage: touch [name] -C content", (*current_line)++, 0, 0x0C);
  }

  print(dir, *current_line, 0, 0x0F);
  print(">", *current_line, 1, 0x0F);

  if (*current_line >= 25) {
    *current_line = 24;
  }
}
