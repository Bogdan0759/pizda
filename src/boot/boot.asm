[org 0x7c00]
KERNEL_OFFSET equ 0x100000  ; Ядро теперь загружается на 1MB

mov [BOOT_DRIVE], dl    

; 1. Читаем ядро с диска
mov ah, 0x02
mov al, 30              ; Читаем больше секторов для 64-бит ядра
mov ch, 0x00
mov dh, 0x00
mov cl, 0x02            
mov bx, 0x8000          ; Временный буфер
int 0x13

; 2. Проверка поддержки long mode
mov eax, 0x80000000
cpuid
cmp eax, 0x80000001
jb no_long_mode

mov eax, 0x80000001
cpuid
test edx, 1 << 29       ; Проверяем бит LM
jz no_long_mode

; 3. Настройка page tables (identity mapping первых 2MB)
; PML4 at 0x1000
mov edi, 0x1000
mov cr3, edi
xor eax, eax
mov ecx, 4096
rep stosd
mov edi, cr3

; PML4[0] -> PDPT
mov DWORD [edi], 0x2003
; PDPT[0] -> PD
mov DWORD [0x2000], 0x3003
; PD[0] -> 2MB page
mov DWORD [0x3000], 0x83

; 4. Включаем PAE
mov eax, cr4
or eax, 1 << 5
mov cr4, eax

; 5. Устанавливаем long mode bit
mov ecx, 0xC0000080
rdmsr
or eax, 1 << 8
wrmsr

; 6. Переход в 32-битный protected mode
cli
lgdt [gdt_descriptor]
mov eax, cr0
or eax, 0x1
mov cr0, eax
jmp 0x08:init_pm

[bits 32]
init_pm:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Копируем ядро на 1MB
    mov esi, 0x8000
    mov edi, KERNEL_OFFSET
    mov ecx, 15 * 512 / 4
    rep movsd

    ; Включаем paging (переход в long mode)
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; Прыгаем в 64-битный код
    jmp 0x18:init_lm

[bits 64]
init_lm:
    mov ax, 0x20
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call KERNEL_OFFSET  ; Прыжок в ядро
    jmp $

no_long_mode:
    jmp $

BOOT_DRIVE db 0

; --- GDT для 64-бит ---
gdt_start:
    dq 0x0                          ; Null descriptor
gdt_code_32:
    dw 0xffff, 0x0000, 0x9a00, 0x00cf  ; 32-bit code
gdt_data_32:
    dw 0xffff, 0x0000, 0x9200, 0x00cf  ; 32-bit data
gdt_code_64:
    dw 0x0000, 0x0000, 0x9a00, 0x0020  ; 64-bit code
gdt_data_64:
    dw 0x0000, 0x0000, 0x9200, 0x0000  ; 64-bit data
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xaa55
