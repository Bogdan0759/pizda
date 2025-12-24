	.intel_syntax noprefix
	.text
	.globl	write_byte
	.globl	write_word
	.globl	write_dword

write_byte:
	test	rdi, rdi
	je	.error
	cmp	rdi, 0x1000
	jb	.error
	mov	BYTE PTR [rdi], sil
	xor	eax, eax
	ret
.error:
	mov	eax, -1
	ret

write_word:
	test	rdi, rdi
	je	.error_w
	cmp	rdi, 0x1000
	jb	.error_w
	mov	WORD PTR [rdi], si
	xor	eax, eax
	ret
.error_w:
	mov	eax, -1
	ret

write_dword:
	test	rdi, rdi
	je	.error_d
	cmp	rdi, 0x1000
	jb	.error_d
	mov	DWORD PTR [rdi], esi
	xor	eax, eax
	ret
.error_d:
	mov	eax, -1
	ret
