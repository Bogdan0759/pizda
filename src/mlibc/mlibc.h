#ifndef MLIBC_H
#define MLIBC_H

int strcmp(const char *str1, const char *str2);
int atoi(const char *str);
char *itoa(int value, char *str, int base);
void printf(char *format, ...);
void outb(unsigned short port, unsigned char data);
unsigned char inb(unsigned short port);
void outw(unsigned short port, unsigned short data);
unsigned short inw(unsigned short port);
void insw(unsigned short port, void *addr, unsigned long count);
void outsw(unsigned short port, void *addr, unsigned long count);

#include <mlibc/memory.h>

#endif