#include <kernel/bin/calc.h>
#include <kernel/bin/cat.h>
#include <kernel/bin/disk.h>
#include <kernel/bin/info.h>
#include <kernel/bin/mydir.h>
#include <kernel/bin/fetch.h>
#include <kernel/bin/shell.h>
#include <kernel/bin/touch.h>
#include <kernel/drivers/fs/chainFS/chainfs.h>
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
    print("/>", *current_line, 0, 0x0F);
  } else if (strcmp(s_buf, "clear") == 0) {
    clear_screen();
    *current_line = 1;
    print("/>", *current_line, 0, 0x0F);
  } else if (strcmp(s_buf, "help") == 0) {
    print("Commands: shutdown, reboot, clear, echo, mydir, help, calc, info,",
          (*current_line)++, 0, 0x0A);
    print("          test_fs, mkdir, ls, cd, rmdir, touch, cat, format, disk",
          (*current_line)++, 0, 0x0A);
    print("/>", *current_line, 0, 0x0F);
  } else if (s_buf[0] == 'e' && s_buf[1] == 'c' && s_buf[2] == 'h' &&
             s_buf[3] == 'o' && s_buf[4] == ' ') {
    print(&s_buf[5], *current_line, 2, 0x0F);
    (*current_line)++;
    print("/>", *current_line, 0, 0x0F);
  } else if (strcmp(s_buf, "calc") == 0) {
    calc(s_buf, current_line, dir);
  } 
  else if (strcmp(s_buf, "fetch") == 0) {
    fetch(current_line);
  } 
  else if (strcmp(s_buf, "disk") == 0 ||
             (s_buf[0] == 'd' && s_buf[1] == 'i' && s_buf[2] == 's' &&
              s_buf[3] == 'k' && s_buf[4] == ' ')) {
    disk(s_buf, current_line, dir);
  } else if (strcmp(s_buf, "mkdir") == 0) {
    print("Usage: mkdir <directory_name>", (*current_line)++, 0, 0x0C);
    print("/>", *current_line, 0, 0x0F);
  } else if (s_buf[0] == 'm' && s_buf[1] == 'k' && s_buf[2] == 'd' &&
             s_buf[3] == 'i' && s_buf[4] == 'r' && s_buf[5] == ' ') {
    if (chainfs_mkdir(&s_buf[6]) == 0) {
      print("Directory created", (*current_line)++, 0, 0x0A);
    } else {
      print("Failed to create directory", (*current_line)++, 0, 0x0C);
    }
    print("/>", *current_line, 0, 0x0F);
  } else if (strcmp(s_buf, "ls") == 0) {
    chainfs_file_entry_t file_list[20];
    u32 file_count;
    if (chainfs_list_dir("", file_list, 20, &file_count) == 0) {
      if (file_count == 0) {
        print("Directory is empty", (*current_line)++, 0, 0x0F);
      } else {
        for (u32 i = 0; i < file_count; i++) {
          char file_info[80];
          int pos = 0;

          if (file_list[i].type == CHAINFS_TYPE_DIR) {
            file_info[pos++] = '[';
            file_info[pos++] = 'D';
            file_info[pos++] = 'I';
            file_info[pos++] = 'R';
            file_info[pos++] = ']';
          } else {
            file_info[pos++] = '[';
            file_info[pos++] = 'F';
            file_info[pos++] = 'I';
            file_info[pos++] = 'L';
            file_info[pos++] = 'E';
            file_info[pos++] = ']';
          }
          file_info[pos++] = ' ';

          int name_len = strlen(file_list[i].name);
          for (int j = 0; j < name_len; j++) {
            file_info[pos++] = file_list[i].name[j];
          }

          if (file_list[i].type == CHAINFS_TYPE_FILE) {
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
          }
          file_info[pos] = 0;

          print(file_info, (*current_line)++, 0, 0x0F);
        }
      }
    } else {
      print("Error reading directory", (*current_line)++, 0, 0x0C);
    }
    print("/>", *current_line, 0, 0x0F);
  } else if (s_buf[0] == 'l' && s_buf[1] == 's' && s_buf[2] == ' ') {
    chainfs_file_entry_t file_list[20];
    u32 file_count;
    if (chainfs_list_dir(&s_buf[3], file_list, 20, &file_count) == 0) {
      if (file_count == 0) {
        print("Directory is empty", (*current_line)++, 0, 0x0F);
      } else {
        for (u32 i = 0; i < file_count; i++) {
          char file_info[80];
          int pos = 0;

          if (file_list[i].type == CHAINFS_TYPE_DIR) {
            file_info[pos++] = '[';
            file_info[pos++] = 'D';
            file_info[pos++] = 'I';
            file_info[pos++] = 'R';
            file_info[pos++] = ']';
          } else {
            file_info[pos++] = '[';
            file_info[pos++] = 'F';
            file_info[pos++] = 'I';
            file_info[pos++] = 'L';
            file_info[pos++] = 'E';
            file_info[pos++] = ']';
          }
          file_info[pos++] = ' ';

          int name_len = strlen(file_list[i].name);
          for (int j = 0; j < name_len; j++) {
            file_info[pos++] = file_list[i].name[j];
          }

          if (file_list[i].type == CHAINFS_TYPE_FILE) {
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
          }
          file_info[pos] = 0;

          print(file_info, (*current_line)++, 0, 0x0F);
        }
      }
    } else {
      print("Error reading directory", (*current_line)++, 0, 0x0C);
    }
    print("/>", *current_line, 0, 0x0F);
  } else if (strcmp(s_buf, "cd") == 0) {
    if (chainfs_chdir("/") == 0) {
      chainfs_get_current_path(dir, 256);
      print("Changed to root directory", (*current_line)++, 0, 0x0A);
    } else {
      print("Failed to change to root directory", (*current_line)++, 0, 0x0C);
    }
    print("/>", *current_line, 0, 0x0F);
  } else if (s_buf[0] == 'c' && s_buf[1] == 'd' && s_buf[2] == ' ') {
    if (chainfs_chdir(&s_buf[3]) == 0) {
      chainfs_get_current_path(dir, 256);
      print("Changed directory", (*current_line)++, 0, 0x0A);
    } else {
      print("Failed to change directory", (*current_line)++, 0, 0x0C);
    }
    print("/>", *current_line, 0, 0x0F);
  } else if (strcmp(s_buf, "rmdir") == 0) {
    print("Usage: rmdir <directory_name>", (*current_line)++, 0, 0x0C);
    print("/>", *current_line, 0, 0x0F);
  } else if (s_buf[0] == 'r' && s_buf[1] == 'm' && s_buf[2] == 'd' &&
             s_buf[3] == 'i' && s_buf[4] == 'r' && s_buf[5] == ' ') {
    if (chainfs_rmdir(&s_buf[6]) == 0) {
      print("Directory removed", (*current_line)++, 0, 0x0A);
    } else {
      print("Failed to remove directory", (*current_line)++, 0, 0x0C);
    }
    print("/>", *current_line, 0, 0x0F);
  } else if (s_buf[0] == 't' && s_buf[1] == 'o' && s_buf[2] == 'u' &&
             s_buf[3] == 'c' && s_buf[4] == 'h' &&
             (s_buf[5] == ' ' || s_buf[5] == 0)) {
    touch(s_buf, current_line, dir);
  } else if (s_buf[0] == 'c' && s_buf[1] == 'a' && s_buf[2] == 't' &&
             (s_buf[3] == ' ' || s_buf[3] == 0)) {
    cat(s_buf, current_line, dir);
  } else if (strcmp(s_buf, "format") == 0) {
    chainfs_init();
    if (chainfs_format(20480, 100) == 0) {
      chainfs_get_current_path(dir, 256);
    } else {
      print("format failed", (*current_line)++, 0, 0x0C);
    }
    print("/>", *current_line, 0, 0x0F);
  } else if (strcmp(s_buf, "test_fs") == 0) {
    print("=== ChainFS Directory Test ===", (*current_line)++, 0, 0x0A);

    if (chainfs_format(20480, 100) != 0) {
      print("format failed", (*current_line)++, 0, 0x0C);
    } else {
      print("format ok", (*current_line)++, 0, 0x0A);
    }

    // Test 1: Create directories
    print("Creating directories...", (*current_line)++, 0, 0x0F);
    if (chainfs_mkdir("test") == 0) {
      print("  mkdir test - OK", (*current_line)++, 0, 0x0A);
    } else {
      print("  mkdir test - FAILED", (*current_line)++, 0, 0x0C);
    }

    if (chainfs_mkdir("test/subdir") == 0) {
      print("  mkdir test/subdir - OK", (*current_line)++, 0, 0x0A);
    } else {
      print("  mkdir test/subdir - FAILED", (*current_line)++, 0, 0x0C);
    }

    // Test 2: Create files in root
    print("Creating files in root...", (*current_line)++, 0, 0x0F);
    const char *test_data = "root file content";
    if (chainfs_write_file("root.txt", (const u8 *)test_data,
                           strlen(test_data)) == 0) {
      print("  root.txt - OK", (*current_line)++, 0, 0x0A);
    } else {
      print("  root.txt - FAILED", (*current_line)++, 0, 0x0C);
    }

    // Test 3: Create files in test directory
    print("Creating files in test dir...", (*current_line)++, 0, 0x0F);
    const char *test_data2 = "test directory file";
    if (chainfs_write_file("test/file1.txt", (const u8 *)test_data2,
                           strlen(test_data2)) == 0) {
      print("  test/file1.txt - OK", (*current_line)++, 0, 0x0A);
    } else {
      print("  test/file1.txt - FAILED", (*current_line)++, 0, 0x0C);
    }

    const char *test_data3 = "subdir file content";
    if (chainfs_write_file("test/subdir/deep.txt", (const u8 *)test_data3,
                           strlen(test_data3)) == 0) {
      print("  test/subdir/deep.txt - OK", (*current_line)++, 0, 0x0A);
    } else {
      print("  test/subdir/deep.txt - FAILED", (*current_line)++, 0, 0x0C);
    }

    // Test 4: List root directory
    print("Root directory contents:", (*current_line)++, 0, 0x0F);
    chainfs_file_entry_t file_list[10];
    u32 file_count;
    if (chainfs_list_dir("/", file_list, 10, &file_count) == 0) {
      for (u32 i = 0; i < file_count; i++) {
        char file_info[80];
        int pos = 0;
        file_info[pos++] = ' ';
        file_info[pos++] = ' ';

        if (file_list[i].type == CHAINFS_TYPE_DIR) {
          file_info[pos++] = '[';
          file_info[pos++] = 'D';
          file_info[pos++] = ']';
        } else {
          file_info[pos++] = '[';
          file_info[pos++] = 'F';
          file_info[pos++] = ']';
        }
        file_info[pos++] = ' ';

        int name_len = strlen(file_list[i].name);
        for (int j = 0; j < name_len; j++) {
          file_info[pos++] = file_list[i].name[j];
        }
        file_info[pos] = 0;

        print(file_info, (*current_line)++, 0, 0x0F);
      }
    }

    // Test 5: List test directory
    print("Test directory contents:", (*current_line)++, 0, 0x0F);
    if (chainfs_list_dir("test", file_list, 10, &file_count) == 0) {
      for (u32 i = 0; i < file_count; i++) {
        char file_info[80];
        int pos = 0;
        file_info[pos++] = ' ';
        file_info[pos++] = ' ';

        if (file_list[i].type == CHAINFS_TYPE_DIR) {
          file_info[pos++] = '[';
          file_info[pos++] = 'D';
          file_info[pos++] = ']';
        } else {
          file_info[pos++] = '[';
          file_info[pos++] = 'F';
          file_info[pos++] = ']';
        }
        file_info[pos++] = ' ';

        int name_len = strlen(file_list[i].name);
        for (int j = 0; j < name_len; j++) {
          file_info[pos++] = file_list[i].name[j];
        }
        file_info[pos] = 0;

        print(file_info, (*current_line)++, 0, 0x0F);
      }
    }

    // Test 6: Read files
    print("Reading files...", (*current_line)++, 0, 0x0F);
    u8 read_buffer[256];
    u32 bytes_read;

    if (chainfs_read_file("root.txt", read_buffer, sizeof(read_buffer),
                          &bytes_read) == 0) {
      read_buffer[bytes_read] = 0;
      print("  root.txt: ", (*current_line), 0, 0x0A);
      print((char *)read_buffer, (*current_line)++, 12, 0x0F);
    }

    if (chainfs_read_file("test/file1.txt", read_buffer, sizeof(read_buffer),
                          &bytes_read) == 0) {
      read_buffer[bytes_read] = 0;
      print("  test/file1.txt: ", (*current_line), 0, 0x0A);
      print((char *)read_buffer, (*current_line)++, 17, 0x0F);
    }

    print("=== Test Complete ===", (*current_line)++, 0, 0x0A);

    print("/>", *current_line, 0, 0x0F);
  } else {
    print("Unknown command", *current_line, 0, 0x0F);
    (*current_line)++;
    print("/>", *current_line, 0, 0x0F);
  }

  if (*current_line >= 25) {
    *current_line = 24;
  }
}