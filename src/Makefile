CC = gcc
LD = ld
AS = nasm
QEMU = qemu-system-x86_64

CFLAGS = -m64 -ffreestanding -fno-pic -fno-stack-protector -O0 -mno-red-zone -mcmodel=kernel
LDFLAGS = -m elf_x86_64 -T linker.ld

KERNEL_BIN = ../bin/kernel.bin
ISO_IMAGE = ../otsos.iso
ISODIR = ../isodir

BOOT_O = ../bin/boot.o
KERNEL_O = ../bin/kernel.o
VGA_O = ../bin/vga.o
KEYBOARD_O = ../bin/keyboard.o
PRINTF_O = ../bin/printf.o
STRING_O = ../bin/string.o
ITOA_O = ../bin/itoa.o
HARDWARE_O = ../bin/hardware.o
READ_O = ../bin/read.o
WRITE_O = ../bin/write.o
CALC_O = ../bin/calc.o  
MYDIR_O = ../bin/mydir.o
INFO_O = ../bin/info.o
SHELL_O = ../bin/shell.o
COM1_O = ../bin/com1.o

OBJ = $(BOOT_O) $(KERNEL_O) $(VGA_O) $(KEYBOARD_O) $(PRINTF_O) $(STRING_O) \
      $(ITOA_O) $(HARDWARE_O) $(READ_O) $(WRITE_O) $(CALC_O) $(MYDIR_O) \
      $(INFO_O) $(SHELL_O) $(COM1_O)

all: $(ISO_IMAGE)

$(ISO_IMAGE): $(KERNEL_BIN)
	mkdir -p $(ISODIR)/boot/grub
	cp $(KERNEL_BIN) $(ISODIR)/boot/kernel.bin
	cp $(ISODIR)/boot/grub/grub.cfg $(ISODIR)/boot/grub/grub.cfg || echo 'menuentry "OTSOS" { multiboot /boot/kernel.bin }' > $(ISODIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_IMAGE) $(ISODIR)

$(KERNEL_BIN): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $@

$(BOOT_O): boot/boot.asm
	$(AS) -f elf64 $< -o $@

$(KERNEL_O): kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

$(VGA_O): kernel/drivers/vga.c
	$(CC) $(CFLAGS) -c $< -o $@

$(KEYBOARD_O): kernel/drivers/keyboard.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PRINTF_O): mlibc/printf.s
	$(CC) -c $< -o $@

$(STRING_O): mlibc/string.c
	$(CC) $(CFLAGS) -c $< -o $@

$(ITOA_O): mlibc/itoa.s
	$(CC) -c $< -o $@

$(HARDWARE_O): mlibc/hardware.asm
	$(AS) -f elf64 $< -o $@ -D__x86_64__

$(READ_O): kernel/memory/read.s
	$(CC) -c $< -o $@

$(WRITE_O): kernel/memory/write.s
	$(CC) -c $< -o $@

$(CALC_O): kernel/func/calc.c
	$(CC) $(CFLAGS) -c $< -o $@

$(INFO_O): kernel/func/info.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MYDIR_O): kernel/func/mydir.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SHELL_O): kernel/func/shell.c
	$(CC) $(CFLAGS) -c $< -o $@

$(COM1_O): LIB/com1.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(ISO_IMAGE)
	$(QEMU) -cdrom $(ISO_IMAGE) -serial stdio

clean:
	rm -f ../bin/*.o $(KERNEL_BIN) $(ISO_IMAGE)
	rm -rf $(ISODIR)/boot/kernel.bin
