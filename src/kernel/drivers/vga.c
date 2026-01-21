#include "vga.h"
#include "../../mlibc/mlibc.h"
#include "../../LIB/com1.h"
// Тупо очистка
void clear_screen() {
    char* video_memory = (char*) 0xb8000;
    for (int i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i+1] = 0x07;
    }
}

// Попытка сделать print
void print(char* message, int row, int col, char color) {
    com1_write_string("{VGA DRIVER}: called print at row ");
    com1_write_hex_dword((u32)row); 
    com1_write_string(" col ");
    com1_write_hex_dword((u32)col); 
    com1_newline();
    
    char* video_memory = (char*) 0xb8000;
    int offset = (row * 80 + col) * 2;
    int i = 0;
    while (message[i] != 0) {
        video_memory[offset + (i * 2)] = message[i];
        // Красивый цвет
        video_memory[offset + (i * 2) + 1] = color;
        i++;
    }
}

void update_cursor(int x, int y) {
    // Вычисляем позицию в одномерном массиве (0-2000 для 80x25)
    unsigned short pos = y * 80 + x;

    // Говорим видеокарте, что хотим отправить младший байт позиции (регистр 15)
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));

    // Говорим видеокарте, что хотим отправить старший байт позиции (регистр 14)
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

