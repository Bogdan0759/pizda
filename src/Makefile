CC = gcc
LD = ld
AS = nasm
QEMU = qemu-system-x86_64

CFLAGS = -m64 -ffreestanding -fno-pic -fno-stack-protector -g -O0 -mno-red-zone -mcmodel=kernel -I.
LDFLAGS = -m elf_x86_64 -T linker.ld -Map=../bin/kernel.map

KERNEL_BIN = ../bin/kernel.bin
ISO_IMAGE = ../otsos.iso
ISODIR = ../isodir
DISK_IMG = ../bin/disk.img
# object files
BOOT_O = ../bin/boot.o
KERNEL_O = ../bin/kernel.o
VGA_O = ../bin/vga.o
KEYBOARD_O = ../bin/keyboard.o
PRINTF_O = ../bin/printf.o
STRING_O = ../bin/string.o
ITOA_O = ../bin/itoa.o
HARDWARE_O = ../bin/hardware.o
CALC_O = ../bin/calc.o 
FETCH_O = ../bin/fetch.o  
MYDIR_O = ../bin/mydir.o
INFO_O = ../bin/info.o
SHELL_O = ../bin/shell.o
COM1_O = ../bin/com1.o
IDT_ASM_O = ../bin/idt_asm.o
IDT_C_O = ../bin/idt_c.o
PIC_O = ../bin/pic_c.o
HANDLERS_O = ../bin/handlers_c.o
PANIC_O = ../bin/panic.o
MEMORY_O = ../bin/memory.o
PATA_O = ../bin/pata.o
CHAINFS_O = ../bin/chainfs.o
CAT_O = ../bin/cat.o
TOUCH_O = ../bin/touch.o
DISK_O = ../bin/disk.o
MOUSE_O = ../bin/mouse.o

OBJ = $(BOOT_O) $(KERNEL_O) $(VGA_O) $(KEYBOARD_O) $(PRINTF_O) $(STRING_O) \
      $(ITOA_O) $(HARDWARE_O) $(CALC_O) $(FETCH_O) $(MYDIR_O) \
      $(INFO_O) $(SHELL_O) $(COM1_O) $(IDT_ASM_O) $(IDT_C_O) $(PIC_O) \
      $(HANDLERS_O) $(PANIC_O) $(MEMORY_O) $(PATA_O) $(CHAINFS_O) $(CAT_O) $(TOUCH_O) $(DISK_O) $(MOUSE_O)

all: $(ISO_IMAGE)

$(ISO_IMAGE): $(KERNEL_BIN)
	mkdir -p $(ISODIR)/boot/grub
	cp $(KERNEL_BIN) $(ISODIR)/boot/kernel.bin
	cp $(ISODIR)/boot/grub/grub.cfg $(ISODIR)/boot/grub/grub.cfg || echo 'menuentry "OTSOS" { multiboot /boot/kernel.bin }' > $(ISODIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_IMAGE) $(ISODIR)

$(KERNEL_BIN): $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $@

$(BOOT_O): boot/boot.asm
	$(AS) -f elf64 -g -F dwarf $< -o $@

$(IDT_ASM_O): kernel/interrupts/idt.asm
	$(AS) -f elf64 -g -F dwarf $< -o $@

$(KERNEL_O): kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

$(IDT_C_O): kernel/interrupts/idt.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PIC_O): kernel/interrupts/pic.c
	$(CC) $(CFLAGS) -c $< -o $@

$(HANDLERS_O): kernel/interrupts/handlers.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PANIC_O): kernel/panic.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MEMORY_O): mlibc/memory.c
	$(CC) $(CFLAGS) -c $< -o $@

$(PATA_O): kernel/drivers/disk/pata/pata.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CHAINFS_O): kernel/drivers/fs/chainFS/chainfs.c
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
	$(AS) -f elf64 -g -F dwarf $< -o $@ -D__x86_64__



$(CALC_O): kernel/bin/calc.c
	$(CC) $(CFLAGS) -c $< -o $@
$(FETCH_O): kernel/bin/fetch.c
	$(CC) $(CFLAGS) -c $< -o $@
$(INFO_O): kernel/bin/info.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MYDIR_O): kernel/bin/mydir.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SHELL_O): kernel/bin/shell.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CAT_O): kernel/bin/cat.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TOUCH_O): kernel/bin/touch.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DISK_O): kernel/bin/disk.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MOUSE_O): kernel/drivers/mouse.c
	$(CC) $(CFLAGS) -c $< -o $@

$(COM1_O): lib/com1.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DISK_IMG):
	dd if=/dev/zero of=$(DISK_IMG) bs=1M count=10

run: $(ISO_IMAGE) $(DISK_IMG)
	$(QEMU) -cdrom $(ISO_IMAGE) -drive file=$(DISK_IMG),format=raw,index=0,media=disk -serial stdio

debug: $(ISO_IMAGE) $(DISK_IMG)
	$(QEMU) -cdrom $(ISO_IMAGE) -drive file=$(DISK_IMG),format=raw,index=0,media=disk -serial stdio -d int,cpu_reset -no-reboot

clean:
	rm -f ../bin/*.o ../bin/kernel.bin ../bin/kernel.map $(ISO_IMAGE) $(DISK_IMG)
	rm -rf $(ISODIR)/boot/kernel.bin
