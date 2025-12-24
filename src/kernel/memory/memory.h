#ifndef MEMORY_H
#define MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif
unsigned char read_byte(void* addr);
unsigned short read_word(void* addr);
unsigned int read_dword(void* addr);
int write_byte(void* addr, unsigned char value);
int write_word(void* addr, unsigned short value);
int write_dword(void* addr, unsigned int value);

#ifdef __cplusplus
}
#endif














#define write(addr, value) _Generic((value), \
    char: write_byte, \
    signed char: write_byte, \
    unsigned char: write_byte, \
    short: write_word, \
    unsigned short: write_word, \
    int: write_dword, \
    unsigned int: write_dword, \
    long: write_dword, \
    unsigned long: write_dword, \
    default: write_dword \
)(addr, value)
#define read(type, addr) _Generic((type){0}, \
    char: read_byte, \
    signed char: read_byte, \
    unsigned char: read_byte, \
    short: read_word, \
    unsigned short: read_word, \
    int: read_dword, \
    unsigned int: read_dword, \
    long: read_dword, \
    unsigned long: read_dword, \
    default: read_dword \
)(addr)




typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;


















#endif
