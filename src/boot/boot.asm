[bits 32]
section .multiboot
align 4
    dd 0x1BADB002        ; Magic number
    dd 0x00              ; Flags
    dd -(0x1BADB002 + 0x00) ; Checksum

section .text
global start
extern kmain

start:
    cli
    mov esp, stack_top

    ; 1. Проверка на поддержку 64-бит (CPUID)
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    cmp eax, ecx
    je no_cpuid

    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb no_long_mode

    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz no_long_mode

    ; 2. Настройка Paging (для перехода в 64-бит)
    mov eax, p3_table
    or eax, 0b11 ; present + writable
    mov [p4_table], eax

    mov eax, p2_table
    or eax, 0b11 ; present + writable
    mov [p3_table], eax

    ; Identity mapping первых 2MiB
    mov eax, 0
    or eax, 0b10000011 ; present + writable + huge
    mov [p2_table], eax

    ; Включаем PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Устанавливаем Long Mode bit в EFER MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Включаем Paging
    mov eax, cr3
    mov eax, p4_table
    mov cr3, eax

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; 3. Загружаем 64-бит GDT и прыгаем
    lgdt [gdt64.pointer]
    jmp gdt64.code:init_64

[bits 64]
init_64:
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Прыгаем в C!
    call kmain
    hlt

no_cpuid:
    mov dword [0xb8000], 0x4f524f45 ; "ER"
    hlt

no_long_mode:
    mov dword [0xb8000], 0x4f4d4f4c ; "LM"
    hlt

section .bss
align 4096
p4_table:
    resb 4096
p3_table:
    resb 4096
p2_table:
    resb 4096
stack_bottom:
    resb 4096 * 4
stack_top:

section .rodata
gdt64:
    dq 0 ; zero entry
.code: equ $ - gdt64
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment
.pointer:
    dw $ - gdt64 - 1
    dq gdt64
