# Boot Process

OTSOS uses a standard Multiboot-compliant bootloader (like GRUB) to transition from the bootloader to the kernel.

## 1. Multiboot Header
The kernel starts with a Multiboot header in `boot.asm` located in the `.multiboot` section.
- Magic: `0x1BADB002`
- Flags: `0x00`
- Checksum: Calculated according to Multiboot spec.

## 2. Transition to 64-bit (Long Mode)
Since OTSOS is an x86_64 OS, the boot process involves several critical steps to enter Long Mode:

### CPUID Verification
The bootloader start code (`start` label) checks if the CPU supports:
1. **CPUID instruction**: Verified by attempting to flip the ID bit in `EFLAGS`.
2. **Long Mode**: Verified using `CPUID` extended function `0x80000001`.

### Paging Setup
A basic 4-level paging structure is initialized:
- `p4_table`: Page Map Level 4.
- `p3_table`: Page Directory Pointer Table.
- `p2_table`: Page Directory.
The first 2MiB of memory is identity-mapped using a "Huge Page" (2MiB) entry in the `p2_table`.

### Enabling Long Mode
1. **PAE (Physical Address Extension)**: Enabled in `CR4`.
2. **EFER MSR**: The Long Mode Bit is set.
3. **Paging Enabled**: `CR0` bit 31 is set.

## 3. Kernel Entry
A 64-bit GDT is loaded, and a long jump transitions the execution to the `init_64` label (64-bit code). From there, segments are reset, and the control is passed to the C function `kmain`.

## Summary
| Step | Action |
| :--- | :--- |
| 1 | GRUB loads kernel at 1MB |
| 2 | Check for x86_64 support |
| 3 | Setup basic Paging (Identity Map) |
| 4 | Switch to Long Mode |
| 5 | Call `kmain()` in `kernel.c` |
