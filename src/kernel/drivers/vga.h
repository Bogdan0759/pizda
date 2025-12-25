#ifndef VGA_H
#define VGA_H

void clear_screen();
void print(const char* message, int row, int col, char color);
void update_cursor(int x, int y);
unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char data);

#endif