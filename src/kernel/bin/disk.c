#include <kernel/bin/disk.h>
#include <kernel/drivers/disk/pata/pata.h>
#include <kernel/drivers/fs/chainFS/chainfs.h>
#include <kernel/drivers/vga.h>
#include <mlibc/mlibc.h>

void disk(char *s_buf, int *current_line, char *dir) {
  static unsigned short identify_data[256];
  if (s_buf[0] == 'd' && s_buf[1] == 'i' && s_buf[2] == 's' &&
      s_buf[3] == 'k') {
    if (s_buf[4] == ' ' && s_buf[5] == 'i' && s_buf[6] == 'n' &&
        s_buf[7] == 'f' && s_buf[8] == 'o') {
      pata_identify(identify_data);

      print("disk info:", (*current_line)++, 0, 0x0E);

      char serial[21];
      char model[41];
      for (int i = 0; i < 10; i++) {
        serial[i * 2] = (char)(identify_data[10 + i] >> 8);
        serial[i * 2 + 1] = (char)(identify_data[10 + i] & 0xFF);
      }
      serial[20] = 0;

      for (int i = 0; i < 20; i++) {
        model[i * 2] = (char)(identify_data[27 + i] >> 8);
        model[i * 2 + 1] = (char)(identify_data[27 + i] & 0xFF);
      }
      model[40] = 0;

      print("  model: ", (*current_line), 0, 0x0F);
      print(model, (*current_line)++, 10, 0x07);

      print("  serial: ", (*current_line), 0, 0x0F);
      print(serial, (*current_line)++, 11, 0x07);

      unsigned int total_sectors = *((unsigned int *)&identify_data[60]);
      char cap_info[64];
      strcpy(cap_info, "  capacity: ");
      char sect_str[16];
      itoa(total_sectors, sect_str, 10);
      strcat(cap_info, sect_str);
      strcat(cap_info, " sectors (");
      itoa((total_sectors * 512) / (1024 * 1024), sect_str, 10);
      strcat(cap_info, sect_str);
      strcat(cap_info, " MB)");
      print(cap_info, (*current_line)++, 0, 0x0F);

    } else if (s_buf[4] == ' ' && s_buf[5] == 'c' && s_buf[6] == 'l' &&
               s_buf[7] == 'e' && s_buf[8] == 'a' && s_buf[9] == 'r' &&
               s_buf[10] == ' ') {
      unsigned int sector = atoi(&s_buf[11]);
      static unsigned char empty[512];
      memset(empty, 0, 512);
      pata_write_sector(sector, empty);
      print("sector cleared", (*current_line)++, 0, 0x0A);

    } else if (s_buf[4] == ' ' && s_buf[5] == 'u' && s_buf[6] == 's' &&
               s_buf[7] == 'a' && s_buf[8] == 'g' && s_buf[9] == 'e') {
      chainfs_superblock_t sb;
      // We don't have a direct 'get_superblock' but we can read sector 0
      static unsigned char usage_buf[512];
      pata_read_sector(0, usage_buf);
      memcpy(&sb, usage_buf, sizeof(chainfs_superblock_t));

      if (sb.magic == CHAINFS_MAGIC) {
        print("chainfs usage:", (*current_line)++, 0, 0x0E);
        char stats[64];

        strcpy(stats, "  total blocks: ");
        itoa(sb.block_count, stats + strlen(stats), 10);
        print(stats, (*current_line)++, 0, 0x0F);

        strcpy(stats, "  max files: ");
        itoa(sb.total_files, stats + strlen(stats), 10);
        print(stats, (*current_line)++, 0, 0x0F);
      } else {
        print("init fs first", (*current_line)++, 0, 0x0C);
      }

    } else if (s_buf[4] == ' ' && s_buf[5] == 'd' && s_buf[6] == 'u' &&
               s_buf[7] == 'm' && s_buf[8] == 'p' && s_buf[9] == ' ') {
      unsigned int sector = atoi(&s_buf[10]);
      static unsigned char dump_buffer[512];
      pata_read_sector(sector, dump_buffer);

      char sector_info[32];
      strcpy(sector_info, "dump");
      char sect_num[16];
      itoa(sector, sect_num, 10);
      strcat(sector_info, sect_num);
      print(sector_info, (*current_line)++, 0, 0x0E);

      for (int i = 0; i < 16; i++) {
        char dump_line[80];
        int pos = 0;

        char addr[8];
        itoa(i * 16, addr, 16);
        int alen = strlen(addr);
        for (int k = 0; k < 4 - alen; k++)
          dump_line[pos++] = '0';
        for (int k = 0; k < alen; k++)
          dump_line[pos++] = addr[k];
        dump_line[pos++] = ':';
        dump_line[pos++] = ' ';

        for (int j = 0; j < 16; j++) {
          unsigned char val = dump_buffer[i * 16 + j];
          char hex[3];
          itoa(val, hex, 16);
          if (val < 16) {
            dump_line[pos++] = '0';
            dump_line[pos++] = hex[0];
          } else {
            dump_line[pos++] = hex[0];
            dump_line[pos++] = hex[1];
          }
          dump_line[pos++] = ' ';
        }

        dump_line[pos++] = '|';
        for (int j = 0; j < 16; j++) {
          unsigned char val = dump_buffer[i * 16 + j];
          if (val >= 32 && val <= 126) {
            dump_line[pos++] = (char)val;
          } else {
            dump_line[pos++] = '.';
          }
        }
        dump_line[pos++] = '|';
        dump_line[pos] = 0;

        print(dump_line, (*current_line)++, 0, 0x0F);
      }
    } else {
      print("use disk info", (*current_line)++, 0, 0x0C);
      print("       disk dump <sector>", (*current_line)++, 0, 0x0C);
      print("       disk clear <sector>", (*current_line)++, 0, 0x0C);
      print("       disk usage", (*current_line)++, 0, 0x0C);
    }
  }

  if (*current_line >= 25) {
    *current_line = 24;
  }
  print("/>", *current_line, 0, 0x0F);
}
