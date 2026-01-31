#include <kernel/drivers/keyboard.h>
#include <kernel/drivers/vga.h>
#include <mlibc/mlibc.h>

unsigned char kbd_us[128] = {
    0,   27,   '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-',
    '=', '\b', '\t', 'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
    '[', ']',  '\n', 0,   'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
    ';', '\'', '`',  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
    '.', '/',  0,    '*', 0,    ' ', 0,   0,   0,   0,   0,   0,   0,
    0,   0,    0,    0,   0,    0,   '7', '8', '9', '-', // Numpad 7,8,9,-
    '4', '5',  '6',  '+',                                // Numpad 4,5,6,+
    '1', '2',  '3',                                      // Numpad 1,2,3
    '0', '.'                                             // Numpad 0,.
};

unsigned char kbd_us_shift[128] = {
    0,    27,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
    '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
    '\n', 0,    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0,    '|',  'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,   '*',
    0,    ' ',  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,    '7',  '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.'};

static int shift_pressed = 0;

void handle_keyboard() {
  static int current_col = 2;
  static int current_row = 5;

  if (inb(0x64) & 0x01) {
    unsigned char scancode = inb(0x60);

    if (scancode == 0x2A || scancode == 0x36) {
      shift_pressed = 1;
      return;
    }
    if (scancode == 0xAA || scancode == 0xB6) {
      shift_pressed = 0;
      return;
    }

    if (scancode < 0x80) {

      char character =
          shift_pressed ? kbd_us_shift[scancode] : kbd_us[scancode];

      if (scancode == 0x48) {
        if (current_row > 0)
          current_row--;
        current_col = 2;
      } else if (scancode == 0x50) {
        if (current_row < 24)
          current_row++;
        current_col = 2;
      } else if (scancode == 0x4B) {
        if (current_col > 2)
          current_col--;
      } else if (scancode == 0x4D) {
        if (current_col < 79)
          current_col++;
      } else {
        if (character == '\n') {
          current_row++;
          current_col = 2;
        } else if (scancode == 0x0E) {
          if (current_col > 0) {
            current_col--;
          }

          char *video_memory = (char *)0xb8000;
          int offset = (current_row * 80 + current_col) * 2;
          video_memory[offset] = ' ';
          video_memory[offset + 1] = 0x07;
        } else if (character != 0) {
          char *video_memory = (char *)0xb8000;
          int offset = (current_row * 80 + current_col) * 2;

          video_memory[offset] = character;
          video_memory[offset + 1] = 0x0E;

          current_col++;
        }
      }
      update_cursor(current_col, current_row);
    }
  }
}

char *scanf(char *buffer, int max_len, int row, int col, char color,
            char *dir) {
  char *video_memory = (char *)0xb8000;
  int offset = (row * 80 + col) * 2;
  int i = 0;

  while (i < max_len - 1) {
    u8 status = inb(0x64);
    if (status & 0x01) {
      if (status & 0x20) {
        (void)inb(0x60);
        continue;
      }
      unsigned char scancode = inb(0x60);

      if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        continue;
      }
      if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
        continue;
      }

      if (scancode < 0x80) {
        // если получу ентер то нужно заканчивать строку
        if (scancode == 0x1C) {
          buffer[i] = '\0';
          update_cursor(col, row + 2);
          return buffer;
        }
        // стирать если это Backspace
        else if (scancode == 0x0E) {
          if (i > 0) {
            i--;
            video_memory[offset + (i * 2)] = ' ';
            video_memory[offset + (i * 2) + 1] = color;
            update_cursor(col + i, row);
          }
        }
        // обычные буковки
        else {
          char character =
              shift_pressed ? kbd_us_shift[scancode] : kbd_us[scancode];

          if (character != 0 && character != '\n') {
            buffer[i] = character;
            video_memory[offset + (i * 2)] = character;
            video_memory[offset + (i * 2) + 1] = color;
            i++;
            print("/>", row, 0, 0x0F);
            update_cursor(col + i, row);
          }
        }
      }
    }
  }

  buffer[i] = '\0';
  return buffer; // возврат того что было в буфере
}
