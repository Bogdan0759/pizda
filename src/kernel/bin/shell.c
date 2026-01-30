#include <kernel/bin/calc.h>
#include <kernel/bin/info.h>
#include <kernel/bin/mydir.h>
#include <kernel/bin/shell.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/drivers/vga.h>
#include <kernel/drivers/fs/chainFS/chainfs.h>
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
    print("Commands: shutdown, reboot, clear, echo, mydir, help, calc, info, test_fs",
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
  } else if (strcmp(s_buf, "test_fs") == 0) {
    
    if (chainfs_format(20480, 100) != 0) {
      print("Format failed!", (*current_line)++, 0, 0x0C);
    } else {
    }
    
    const char* test_data = "test file.";
    if (chainfs_write_file("test.txt", (const u8*)test_data, strlen(test_data)) != 0) {
      print("Write failed!", (*current_line)++, 0, 0x0C);
    } else {
    }
    
    u8 read_buffer[256];
    u32 bytes_read;
    if (chainfs_read_file("test.txt", read_buffer, sizeof(read_buffer), &bytes_read) != 0) {
      print("Read failed!", (*current_line)++, 0, 0x0C);
    } else {
      read_buffer[bytes_read] = 0; 
      print("Read OK:", (*current_line)++, 0, 0x0A);
      print((char*)read_buffer, (*current_line)++, 0, 0x0F);
    }
    
    print("File list:", (*current_line)++, 0, 0x0F);
    chainfs_file_entry_t file_list[10];
    u32 file_count;
    if (chainfs_get_file_list(file_list, 10, &file_count) == 0) {
      if (file_count == 0) {
        print("  No files found", (*current_line)++, 0, 0x0F);
      } else {
        for (u32 i = 0; i < file_count; i++) {
          char file_info[80];
          int pos = 0;
          file_info[pos++] = ' ';
          file_info[pos++] = ' ';
          
          int name_len = strlen(file_list[i].name);
          for (int j = 0; j < name_len; j++) {
            file_info[pos++] = file_list[i].name[j];
          }
          
          file_info[pos++] = ' ';
          file_info[pos++] = '(';
          char size_str[16];
          itoa(file_list[i].size, size_str, 10);
          int size_len = strlen(size_str);
          for (int j = 0; j < size_len; j++) {
            file_info[pos++] = size_str[j];
          }
          file_info[pos++] = ' ';
          file_info[pos++] = 'b';
          file_info[pos++] = 'y';
          file_info[pos++] = 't';
          file_info[pos++] = 'e';
          file_info[pos++] = 's';
          file_info[pos++] = ')';
          file_info[pos] = 0;
          
          print(file_info, (*current_line)++, 0, 0x0F);
        }
      }
    } else {
      print("  Error reading file list", (*current_line)++, 0, 0x0C);
    }
    
    if (chainfs_delete_file("test.txt") != 0) {
      print("Delete failed!", (*current_line)++, 0, 0x0C);
    } else {
    }
    
    print(dir, *current_line, 0, 0x0F);
    print(">", *current_line, 1, 0x0F);
  }

  else {
    print("Unknown command", *current_line, 0, 0x0F);
    (*current_line)++;
    print(dir, *current_line, 0, 0x0F);
    print(">", *current_line, 1, 0x0F);
  }
}