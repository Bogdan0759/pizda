#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "../mlibc/string.h"




// Точка входа(если что-то сломалось то здесь)
__attribute__((section(".text.boot"))) // Помечаем как стартовую секцию
void _start() {
    clear_screen();
    print("LamdaOS started!", 0, 30, 0x0A);
    print("There's nothing here yet =)", 3, 0, 0x0F);

    char s_buf[2048];
    int current_line = 5;
    
    while(1) {
        scanf(s_buf, 2048, current_line, 0, 0x0F);
        current_line++;
        
        if (strcmp(s_buf, "shutdown") == 0) {
            outb(0x64, 0xFE);
            while(1);
        }
        else if (strcmp(s_buf, "reboot") == 0) {
            outb(0x64, 0xFE);
        }
        else if (strcmp(s_buf, "clear") == 0) {
            clear_screen();
            current_line = 5;
        }
        else if (s_buf[0] == 'e' && s_buf[1] == 'c' && s_buf[2] == 'h' && s_buf[3] == 'o' && s_buf[4] == ' ') {
            print(&s_buf[5], current_line, 0, 0x0F);
            current_line++;
        }
        else {
            print("Unknown command", current_line, 0, 0x0F);
            current_line++;
        }
    }
}
