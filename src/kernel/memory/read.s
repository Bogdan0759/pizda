	.intel_syntax noprefix
	.text
	.globl	read_byte
	.globl	read_word
	.globl	read_dword

read_byte:
	test	rdi, rdi
	je	.error_b
	cmp	rdi, 0x1000
	jb	.error_b
	movzx	eax, BYTE PTR [rdi]
	ret
.error_b:
	mov	eax, -1
	ret

read_word:
	test	rdi, rdi
	je	.error_w
	cmp	rdi, 0x1000
	jb	.error_w
	movzx	eax, WORD PTR [rdi]
	ret
.error_w:
	mov	eax, -1
	ret

read_dword:
	test	rdi, rdi
	je	.error_d
	cmp	rdi, 0x1000
	jb	.error_d
	mov	eax, DWORD PTR [rdi]
	ret
.error_d:
	mov	eax, -1
	ret
