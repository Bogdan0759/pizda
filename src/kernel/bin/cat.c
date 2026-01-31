#include <kernel/bin/cat.h>
#include <kernel/drivers/fs/chainFS/chainfs.h>
#include <kernel/drivers/vga.h>
#include <mlibc/mlibc.h>

void cat(char *s_buf, int *current_line, char *dir) {
  if (s_buf[0] == 'c' && s_buf[1] == 'a' && s_buf[2] == 't' &&
      s_buf[3] == ' ') {
    char *filename = &s_buf[4];
    u8 buffer[2048];
    u32 bytes_read;

    if (chainfs_read_file(filename, buffer, sizeof(buffer) - 1, &bytes_read) ==
        0) {
      buffer[bytes_read] = 0;

      char *line = (char *)buffer;
      char *next_line;
      while (line && *line) {
        next_line = strchr(line, '\n');
        if (next_line)
          *next_line = 0;

        print(line, (*current_line)++, 0, 0x0F);

        if (*current_line >= 25) {
          *current_line = 24;
        }

        if (next_line) {
          line = next_line + 1;
        } else {
          line = NULL;
        }
      }
    } else {
      print("failed to read", (*current_line)++, 0, 0x0C);
    }
  } else {
    print("use: cat {filename}", (*current_line)++, 0, 0x0C);
  }

  print("/>", *current_line, 0, 0x0F);

  if (*current_line >= 25) {
    *current_line = 24;
  }
}
