#include <kernel/drivers/keyboard.h>
#include <kernel/drivers/vga.h>
#include <kernel/func/calc.h>
#include <mlibc/mlibc.h>

void calc(char *s_buf, int *current_line, char *dir) {
  // Используем *current_line, так как это указатель
  print(dir, *current_line, 0, 0x0F);
  print(">", *current_line, 1, 0x0F);
  print("first number: ", (*current_line)++, 2, 0x0F);

  // Передаем аргументы дальше
  scanf(s_buf, 2048, *current_line - 1, 16, 0x0F, dir);
  int first_number = atoi(s_buf);

  print(dir, *current_line, 0, 0x0F);
  print(">", *current_line, 1, 0x0F);
  print("second number: ", (*current_line)++, 2, 0x0F);

  scanf(s_buf, 2048, *current_line - 1, 17, 0x0F, dir);
  int second_number = atoi(s_buf);

  print(dir, *current_line, 0, 0x0F);
  print(">", *current_line, 1, 0x0F);
  print("operation: ", (*current_line)++, 2, 0x0F);

  scanf(s_buf, 2048, *current_line - 1, 13, 0x0F, dir);
  char operation = s_buf[0];
  char result_buf[32];

  if (operation == '+') {
    itoa(first_number + second_number, result_buf, 10);
    print(result_buf, (*current_line)++, 0, 0x0F);
  } else if (operation == '-') {
    itoa(first_number - second_number, result_buf, 10);
    print(result_buf, (*current_line)++, 0, 0x0F);
  } else if (operation == '*') {
    itoa(first_number * second_number, result_buf, 10);
    print(result_buf, (*current_line)++, 0, 0x0F);
  } else if (operation == '/') {
    if (second_number != 0) {
      itoa(first_number / second_number, result_buf, 10);
      print(result_buf, (*current_line)++, 0, 0x0F);
    } else {
      print("Error: Div by zero", (*current_line)++, 0, 0x0C);
    }
  }

  print(dir, *current_line, 0, 0x0F);
  print(">", *current_line, 1, 0x0F);
}