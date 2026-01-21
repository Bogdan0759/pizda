#ifndef MLIBC_H
#define MLIBC_H

int strcmp(const char *str1, const char *str2);
int atoi(const char *str);
char *itoa(int value, char *str, int base);
void printf(char *format, ...);
void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);

#include <mlibc/memory.h>

#endif