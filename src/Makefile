# Переменные для инструментов
CC = gcc
LD = ld
AS = nasm
QEMU = qemu-system-x86_64

# Флаги компиляции
CFLAGS = -m64 -ffreestanding -fno-pic -fno-stack-protector -O0 -mno-red-zone -mcmodel=kernel
LDFLAGS = -m elf_x86_64 -T linker.ld

# Имена файлов
BOOT_BIN = ../bin/boot.bin
KERNEL_BIN = ../bin/kernel.bin
KERNEL_O = ../bin/kernel.o
VGA_O = ../bin/vga.o
KEYBOARD_O = ../bin/keyboard.o
PRINTF_O = ../bin/printf.o
STRING_O = ../bin/string.o
ITOA_O = ../bin/itoa.o
HARDWARE_O = ../bin/hardware.o
IMAGE = ../os-image.bin

# Цель по умолчанию (просто собрать образ)
all: $(IMAGE)

# Как собрать образ ОС
$(IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(IMAGE)

# Как собрать загрузчик
$(BOOT_BIN): boot/boot.asm
	$(AS) -f bin boot/boot.asm -o $(BOOT_BIN)

# Как собрать ядро
$(KERNEL_BIN): $(KERNEL_O) $(VGA_O) $(KEYBOARD_O) $(PRINTF_O) $(STRING_O) $(ITOA_O) $(HARDWARE_O)
	$(LD) $(LDFLAGS) $(KERNEL_O) $(VGA_O) $(KEYBOARD_O) $(PRINTF_O) $(STRING_O) $(ITOA_O) $(HARDWARE_O) -o $(KERNEL_BIN)

$(KERNEL_O): kernel/kernel.c
	$(CC) $(CFLAGS) -c kernel/kernel.c -o $(KERNEL_O)

$(VGA_O): kernel/drivers/vga.c
	$(CC) $(CFLAGS) -c kernel/drivers/vga.c -o $(VGA_O)

$(KEYBOARD_O): kernel/drivers/keyboard.c
	$(CC) $(CFLAGS) -c kernel/drivers/keyboard.c -o $(KEYBOARD_O)

$(PRINTF_O): mlibc/printf.c
	$(CC) $(CFLAGS) -c mlibc/printf.c -o $(PRINTF_O)

$(STRING_O): mlibc/string.c
	$(CC) $(CFLAGS) -c mlibc/string.c -o $(STRING_O)

$(ITOA_O): mlibc/itoa.c
	$(CC) $(CFLAGS) -c mlibc/itoa.c -o $(ITOA_O)

$(HARDWARE_O): mlibc/hardware.c
	$(CC) $(CFLAGS) -c mlibc/hardware.c -o $(HARDWARE_O)

# Команда для запуска в QEMU
run: $(IMAGE)
	$(QEMU) -drive format=raw,file=$(IMAGE)

# Команда для очистки проекта
clean:
	rm -f ../bin/*.o ../bin/*.bin $(IMAGE)
