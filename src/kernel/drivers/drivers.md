# Drivers

OTSOS supports several core drivers for hardware interaction, storage, and user input.

## 1. VGA Driver (`src/kernel/drivers/vga.c`)
Responsible for text output to the screen.
- **Mode**: 80x25 Text Mode.
- **Memory Address**: `0xB8000`.
- **API**:
    - `void vga_clear()`: Clears the screen.
    - `void vga_putchar(char c)`: Prints a character at the current cursor position.
    - `void vga_set_color(u8 fg, u8 bg)`: Sets text and background colors.

## 2. Keyboard Driver (`src/kernel/drivers/keyboard.c`)
Handles input from a PS/2 keyboard.
- **Interrupt**: IRQ 1 (Global Interrupt 33).
- **Functionality**:
    - Converts scancodes to ASCII.
    - Supports Shift and CapsLock state.
    - Buffer-based input for the shell.

## 3. Storage: PATA Driver (`src/kernel/drivers/disk/pata/pata.c`)
Driver for IDE/PATA hard drives using PIO mode.
- **Support**: Identify drive, read blocks, and write blocks.
- **API**:
    - `void pata_read_block(u32 lba, u8 *buffer)`
    - `void pata_write_block(u32 lba, u8 *buffer)`

## 4. File System: ChainFS (`src/kernel/drivers/fs/chainFS/chainfs.c`)
The default file system for OTSOS.
- **Type**: Linked-block based file system.
- **Features**:
    - File creation, deletion, reading, and writing.
    - Basic directory support.
- **API**:
    - `int fs_read_file(const char *path, char *buffer)`
    - `int fs_write_file(const char *path, const char *data, int size)`

## 5. Serial Port (COM1) (`src/lib/com1.c`)
Used primarily for kernel debugging and logging.
- **Port**: `0x3F8`.
- **API**:
    - `void com1_printf(const char *format, ...)`: Formatted output to the serial console.
