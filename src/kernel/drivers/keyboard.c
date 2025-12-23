#include "keyboard.h"
#include "vga.h"

unsigned char kbd_us[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',   
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,         
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

void handle_keyboard() {
    static int current_col = 2;
    static int current_row = 5;
    
    if (inb(0x64) & 0x01) {
        unsigned char scancode = inb(0x60);
        
        
        if (scancode < 0x80) {
            
            char character = kbd_us[scancode];
            
            
            if (scancode == 0x48) {       
                if (current_row > 0) current_row--;
                current_col = 2;
            }
            else if (scancode == 0x50) { 
                if (current_row < 24) current_row++;
                current_col = 2;
            }
            else if (scancode == 0x4B) {
                if (current_col > 2) current_col--;
            }
            else if (scancode == 0x4D) { 
                if (current_col < 79) current_col++;
            }
            else {
                if (character == '\n') {
                    current_row++;
                    current_col = 2;
                }
                else if (scancode == 0x0E) { 
                    if (current_col > 0) {
                        current_col--;
                    }
                    
                    char* video_memory = (char*) 0xb8000;
                    int offset = (current_row * 80 + current_col) * 2;
                    video_memory[offset] = ' ';     
                    video_memory[offset + 1] = 0x07; 
                }
                else if (character != 0) {
                    char* video_memory = (char*) 0xb8000;
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


char* scanf(char* buffer, int max_len, int row, int col, char color, char* dir) {
    char* video_memory = (char*) 0xb8000;
    int offset = (row * 80 + col) * 2;
    int i = 0;
    
    while (i < max_len - 1) {
        if (inb(0x64) & 0x01) {
            unsigned char scancode = inb(0x60);
            
            
            if (scancode < 0x80) {
                //если получу ентер то нужно заканчивать строку
                if (scancode == 0x1C) {
                    buffer[i] = '\0';
                    update_cursor(col, row + 2);
                    return buffer;
                }
                //стирать если это Backspace
                else if (scancode == 0x0E) {
                    if (i > 0) {
                        i--;
                        video_memory[offset + (i * 2)] = ' ';
                        video_memory[offset + (i * 2) + 1] = color;
                        update_cursor(col + i, row);
                    }
                }
                //обычные буковки
                else {
                    char character = kbd_us[scancode];
                    
                    if (character != 0 && character != '\n') {
                        buffer[i] = character;
                        video_memory[offset + (i * 2)] = character;
                        video_memory[offset + (i * 2) + 1] = color;
                        i++;
                        print(dir, row,0, 0x0F);
                        print(">", row,1, 0x0F);
                        update_cursor(col + i, row);
                        
                    }
                }
            }
        }
    }
    
    buffer[i] = '\0';
    return buffer; //возврат того что было в буфере
}

