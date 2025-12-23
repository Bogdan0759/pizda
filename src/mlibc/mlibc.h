#ifndef MLIBC_H
#define MLIBC_H
#include "string.h"

int atoi(const char *str) {
  int result = 0;
  int sign = 1;

  if (*str == '-') {
    sign = -1;
    str++;
  }

  while (*str >= '0' && *str <= '9') {
    result = result * 10 + (*str - '0');
    str++;
  }

  return sign * result;
}

void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);

#endif