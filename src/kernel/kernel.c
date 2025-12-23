#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "../mlibc/mlibc.h"




// Точка входа(если что-то сломалось то здесь)
__attribute__((section(".text.boot"))) // Помечаем как стартовую секцию
void _start() {
    clear_screen();
    print("LamdaOS started!", 0, 30, 0x0A);
    print("There's nothing here yet =)", 3, 0, 0x0F);

    char s_buf[2048];
    int current_line = 5;
    char* dir = "/"; //Потом когда добавим файловую систему нужнл будет немного изменить

    print(dir, current_line,0, 0x0F);
    print(">", current_line,1, 0x0F);

    while(1) {
        scanf(s_buf, 2048, current_line, 2, 0x0F, dir);
        current_line++;

        if (strcmp(s_buf, "shutdown") == 0) {
            outb(0x64, 0xFE);
            while(1);
        }
        else if (strcmp(s_buf, "reboot") == 0) {
            outb(0x64, 0xFE);
        }
        else if (strcmp(s_buf, "mydir") == 0) {
            print("Your dir: ", current_line++, 0, 0x0F);
            print(dir, current_line-1,11, 0x0F);
            print(dir, current_line,0, 0x0F);
            print(">", current_line,1, 0x0F);
        }
        else if (strcmp(s_buf, "clear") == 0) {
            clear_screen();
            current_line = 5;
        }
        else if (strcmp(s_buf, "help") == 0) {
            print("Command: shutdown ,reboot, clear, echo, mydir, help", current_line++, 0, 0x0A);
            print(dir, current_line,0, 0x0F);
            print(">", current_line,1, 0x0F);
        }
        else if (s_buf[0] == 'e' && s_buf[1] == 'c' && s_buf[2] == 'h' && s_buf[3] == 'o' && s_buf[4] == ' ') {
            print(&s_buf[5], current_line, 2, 0x0F);
            current_line++;
            print(dir, current_line,0, 0x0F);


        
        } else if (strcmp(s_buf, "calc") == 0) {
            print("firts number: ", current_line++, 0, 0x0F);
            scanf(s_buf, 2048, current_line, 2, 0x0F, dir);
            int first_number = atoi(s_buf);
            print("second number: ", current_line++, 0, 0x0F);
            scanf(s_buf, 2048, current_line, 2, 0x0F, dir);
            int second_number = atoi(s_buf);
            
            
        } 
        else {
            print("Unknown command", current_line, 0, 0x0F);
            current_line++;
            print(dir, current_line,0, 0x0F);
            print(">", current_line,1, 0x0F);
            
        }
    }
}
