#include "mlibc.h"

char *itoa(int value, char *str,
           int base) { // я не ебу как это работает я просто взял с ориг glibc и
                       // адаптировал под
  // железо без ос
  char *ptr = str;
  char *ptr1 = str;
  char tmp_char;
  int tmp_value;

  if (base < 2 || base > 36) {
    *str = '\0';
    return str;
  }

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrst"
             "uvwxyz"[35 + (tmp_value - value * base)];
  } while (value);

  if (tmp_value < 0)
    *ptr++ = '-';
  *ptr-- = '\0';

  while (ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }

  return str;
}
