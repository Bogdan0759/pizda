section .text
global inb
inb:
%ifdef __x86_64__
    mov dx, di          
    xor eax, eax        
    in al, dx           
    ret
%else
    push ebp
    mov ebp, esp
    mov dx, [ebp+8]     
    xor eax, eax        
    in al, dx           
    pop ebp
    ret
%endif
global outb
outb:
%ifdef __x86_64__
    mov dx, di          
    mov al, sil         
    out dx, al          
    ret
%else
    push ebp
    mov ebp, esp
    mov dx, [ebp+8]     
    mov al, [ebp+12]    
    out dx, al          
    pop ebp
    ret
%endif
