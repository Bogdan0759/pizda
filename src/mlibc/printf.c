#include "../kernel/drivers/vga.h"
#include "mlibc.h"
static int cursor_row = 0;
static int cursor_col = 0;

void printf(char *format, ...) { // 55% кода взято (спизджено) с github🥰🥰
  __builtin_va_list args;
  __builtin_va_start(args, format);
  char *video_memory = (char *)0xb8000;

    while (*format) {
    if (*format == '%') {
      format++; 
      switch (*format) {
      case 'd':
      case 'i': { //обработка int значений
        int num = __builtin_va_arg(args, int);
        if (num == 0) {
          video_memory[(cursor_row * 80 + cursor_col) * 2] = '0';
          video_memory[(cursor_row * 80 + cursor_col) * 2 + 1] = 0x07;
          cursor_col++;
        } else {
          if (num < 0) {
            video_memory[(cursor_row * 80 + cursor_col) * 2] = '-';
            video_memory[(cursor_row * 80 + cursor_col) * 2 + 1] = 0x07;
            cursor_col++;
            num = -num;
          }
          char buffer[12];
          int i = 0;
          while (num > 0) {
            buffer[i++] = '0' + (num % 10);
            num /= 10;
          }
          while (i > 0) {
            video_memory[(cursor_row * 80 + cursor_col) * 2] = buffer[--i];
            video_memory[(cursor_row * 80 + cursor_col) * 2 + 1] = 0x07;
            cursor_col++;
          }
        }
        break;
      }
      case 's': { //обработка строк
        char *str = __builtin_va_arg(args, char *);
        while (*str) {
          if (*str == '\n') {
            cursor_row++;
            cursor_col = 0;
            if (cursor_row >= 25)
              cursor_row = 0;
          } else {
            video_memory[(cursor_row * 80 + cursor_col) * 2] = *str;
            video_memory[(cursor_row * 80 + cursor_col) * 2 + 1] = 0x07;
            cursor_col++;
            if (cursor_col >= 80) {
              cursor_col = 0;
              cursor_row++;
              if (cursor_row >= 25)
                cursor_row = 0;
            }
          }
          str++;
        }
        break;
      }
      case 'c': { //обработка символов
        char c = (char)__builtin_va_arg(args, int);
        video_memory[(cursor_row * 80 + cursor_col) * 2] = c;
        video_memory[(cursor_row * 80 + cursor_col) * 2 + 1] = 0x07;
        cursor_col++;
        break;
      }
      case 'x': { //hex
        unsigned int num = __builtin_va_arg(args, unsigned int);
        if (num == 0) {
          video_memory[(cursor_row * 80 + cursor_col) * 2] = '0';
          video_memory[(cursor_row * 80 + cursor_col) * 2 + 1] = 0x07;
          cursor_col++;
        } else {
          char hex[] = "0123456789abcdef";
          char buffer[9];
          int i = 0;
          while (num > 0) {
            buffer[i++] = hex[num % 16];
            num /= 16;
          }
          while (i > 0) {
            video_memory[(cursor_row * 80 + cursor_col) * 2] = buffer[--i];
            video_memory[(cursor_row * 80 + cursor_col) * 2 + 1] = 0x07;
            cursor_col++;
          }
        }
        break;
      }
      case '%': //просто процент
        video_memory[(cursor_row * 80 + cursor_col) * 2] = '%';
        video_memory[(cursor_row * 80 + cursor_col) * 2 + 1] = 0x07;
        cursor_col++;
        break;
      }
    } else if (*format == '\n') { //новая строка
      cursor_row++;
      cursor_col = 0;
      if (cursor_row >= 25)
        cursor_row = 0;
    } else {
      video_memory[(cursor_row * 80 + cursor_col) * 2] = *format;
      video_memory[(cursor_row * 80 + cursor_col) * 2 + 1] = 0x07;
      cursor_col++;
      if (cursor_col >= 80) {
        cursor_col = 0;
        cursor_row++;
        if (cursor_row >= 25)
          cursor_row = 0;
      }
    }
    format++;
  }

  update_cursor(cursor_col, cursor_row);
  __builtin_va_end(args);
}