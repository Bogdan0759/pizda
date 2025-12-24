	.intel_syntax noprefix
	.text
	.globl	printf
printf:
	push	r12
	push	rbp
	push	rbx
	sub	rsp, 112
	mov	QWORD PTR [rsp+96], r8
	lea	rax, [rsp+144]
	mov	r8d, DWORD PTR cursor_row[rip]
	mov	QWORD PTR [rsp+104], r9
	mov	r9d, DWORD PTR cursor_col[rip]
	mov	QWORD PTR [rsp+72], rsi
	mov	QWORD PTR [rsp+80], rdx
	mov	QWORD PTR [rsp+88], rcx
	movzx	ecx, BYTE PTR [rdi]
	mov	QWORD PTR [rsp+48], rax
	lea	rax, [rsp+64]
	mov	DWORD PTR [rsp+40], 8
	mov	QWORD PTR [rsp+56], rax
	test	cl, cl
	je	.L34
	mov	r10, rdi
	mov	edx, r8d
	mov	edi, r9d
	xor	ebx, ebx
	xor	esi, esi
	jmp	.L32
	.p2align 4,,10
	.p2align 3
.L3:
	cmp	cl, 10
	je	.L53
	lea	eax, [rdx+rdx*4]
	sal	eax, 4
	add	eax, edi
	add	edi, 1
	add	eax, eax
	cdqe
	mov	BYTE PTR [rax+753664], cl
	mov	BYTE PTR [rax+753665], 7
	cmp	edi, 79
	jg	.L53
	mov	rax, r10
	mov	ebx, 1
.L6:
	movzx	ecx, BYTE PTR [rax+1]
	lea	r10, [rax+1]
	test	cl, cl
	je	.L55
.L32:
	lea	rax, [r10+1]
	cmp	cl, 37
	jne	.L3
	movzx	ecx, BYTE PTR [r10+1]
	cmp	cl, 37
	je	.L4
	sub	ecx, 99
	cmp	cl, 21
	ja	.L6
	movzx	ecx, cl
	notrack jmp	[QWORD PTR .L8[0+rcx*8]]
	.section	.rodata
	.align 8
	.align 4
.L8:
	.quad	.L11
	.quad	.L10
	.quad	.L6
	.quad	.L6
	.quad	.L6
	.quad	.L6
	.quad	.L10
	.quad	.L6
	.quad	.L6
	.quad	.L6
	.quad	.L6
	.quad	.L6
	.quad	.L6
	.quad	.L6
	.quad	.L6
	.quad	.L6
	.quad	.L9
	.quad	.L6
	.quad	.L6
	.quad	.L6
	.quad	.L6
	.quad	.L7
	.text
	.p2align 4,,10
	.p2align 3
.L53:
	add	edx, 1
	mov	rax, r10
	mov	ebx, 1
	xor	edi, edi
	mov	esi, 1
	cmp	edx, 24
	jle	.L6
	movzx	ecx, BYTE PTR [rax+1]
	xor	edx, edx
	lea	r10, [rax+1]
	test	cl, cl
	jne	.L32
.L55:
	test	bl, bl
	je	.L39
	mov	DWORD PTR cursor_col[rip], edi
.L33:
	test	sil, sil
	je	.L2
	mov	DWORD PTR cursor_row[rip], edx
	mov	r8d, edx
.L2:
	mov	esi, r8d
	call	update_cursor
	add	rsp, 112
	pop	rbx
	pop	rbp
	pop	r12
	ret
	.p2align 4,,10
	.p2align 3
.L10:
	mov	ecx, DWORD PTR [rsp+40]
	cmp	ecx, 47
	ja	.L12
	mov	r10d, ecx
	add	ecx, 8
	add	r10, QWORD PTR [rsp+56]
	mov	DWORD PTR [rsp+40], ecx
.L13:
	lea	ebx, [rdx+rdx*4]
	mov	ecx, DWORD PTR [r10]
	sal	ebx, 4
	lea	r11d, [rdi+rbx]
	lea	r10d, [r11+r11]
	movsx	rbp, r10d
	lea	r10d, [rdi+1]
	test	ecx, ecx
	jne	.L14
	mov	ecx, 1840
	mov	edi, r10d
	mov	ebx, 1
	mov	WORD PTR [rbp+753664], cx
	jmp	.L6
	.p2align 4,,10
	.p2align 3
.L7:
	mov	ecx, DWORD PTR [rsp+40]
	cmp	ecx, 47
	ja	.L26
	mov	r10d, ecx
	add	r10, QWORD PTR [rsp+56]
	add	ecx, 8
	mov	r10d, DWORD PTR [r10]
	mov	DWORD PTR [rsp+40], ecx
	lea	ecx, [rdx+rdx*4]
	sal	ecx, 4
	test	r10d, r10d
	jne	.L28
.L56:
	add	ecx, edi
	mov	r11d, 1840
	add	edi, 1
	mov	ebx, 1
	add	ecx, ecx
	movsx	rcx, ecx
	mov	WORD PTR [rcx+753664], r11w
	jmp	.L6
	.p2align 4,,10
	.p2align 3
.L9:
	mov	ecx, DWORD PTR [rsp+40]
	cmp	ecx, 47
	ja	.L18
	mov	r10d, ecx
	add	ecx, 8
	add	r10, QWORD PTR [rsp+56]
	mov	DWORD PTR [rsp+40], ecx
.L19:
	mov	r11, QWORD PTR [r10]
	movzx	r10d, BYTE PTR [r11]
	test	r10b, r10b
	je	.L6
	.p2align 4,,10
	.p2align 3
.L20:
	cmp	r10b, 10
	je	.L52
	lea	ecx, [rdx+rdx*4]
	sal	ecx, 4
	add	ecx, edi
	add	edi, 1
	add	ecx, ecx
	movsx	rcx, ecx
	mov	BYTE PTR [rcx+753664], r10b
	mov	BYTE PTR [rcx+753665], 7
	cmp	edi, 79
	jle	.L22
.L52:
	add	edx, 1
	xor	edi, edi
	mov	esi, 1
	cmp	edx, 24
	jle	.L22
	xor	edx, edx
.L22:
	movzx	r10d, BYTE PTR [r11+1]
	add	r11, 1
	test	r10b, r10b
	jne	.L20
	mov	ebx, 1
	jmp	.L6
	.p2align 4,,10
	.p2align 3
.L11:
	mov	ecx, DWORD PTR [rsp+40]
	cmp	ecx, 47
	ja	.L24
	mov	r10d, ecx
	add	ecx, 8
	add	r10, QWORD PTR [rsp+56]
	mov	DWORD PTR [rsp+40], ecx
.L25:
	lea	ecx, [rdx+rdx*4]
	mov	r10d, DWORD PTR [r10]
	mov	ebx, 1
	sal	ecx, 4
	add	ecx, edi
	add	edi, 1
	add	ecx, ecx
	movsx	rcx, ecx
	mov	BYTE PTR [rcx+753664], r10b
	mov	BYTE PTR [rcx+753665], 7
	jmp	.L6
	.p2align 4,,10
	.p2align 3
.L4:
	lea	ecx, [rdx+rdx*4]
	mov	r10d, 1829
	mov	ebx, 1
	sal	ecx, 4
	add	ecx, edi
	add	edi, 1
	add	ecx, ecx
	movsx	rcx, ecx
	mov	WORD PTR [rcx+753664], r10w
	jmp	.L6
	.p2align 4,,10
	.p2align 3
.L39:
	mov	edi, r9d
	jmp	.L33
	.p2align 4,,10
	.p2align 3
.L18:
	mov	r10, QWORD PTR [rsp+48]
	lea	rcx, [r10+8]
	mov	QWORD PTR [rsp+48], rcx
	jmp	.L19
	.p2align 4,,10
	.p2align 3
.L12:
	mov	r10, QWORD PTR [rsp+48]
	lea	rcx, [r10+8]
	mov	QWORD PTR [rsp+48], rcx
	jmp	.L13
	.p2align 4,,10
	.p2align 3
.L26:
	mov	r10, QWORD PTR [rsp+48]
	lea	rcx, [r10+8]
	mov	r10d, DWORD PTR [r10]
	mov	QWORD PTR [rsp+48], rcx
	lea	ecx, [rdx+rdx*4]
	sal	ecx, 4
	test	r10d, r10d
	je	.L56
.L28:
	movdqa	xmm0, XMMWORD PTR .LC3[rip]
	mov	BYTE PTR [rsp+32], 0
	mov	r11d, 1
	movaps	XMMWORD PTR [rsp+16], xmm0
	.p2align 4,,10
	.p2align 3
.L29:
	mov	ebx, r10d
	and	ebx, 15
	shr	r10d, 4
	movzx	ebx, BYTE PTR [rsp+16+rbx]
	mov	BYTE PTR [rsp+6+r11], bl
	mov	rbx, r11
	lea	r11, [r11+1]
	jne	.L29
	add	ecx, edi
	movsx	r10, ebx
	add	ecx, ecx
	movsx	rcx, ecx
	add	rcx, 753664
	.p2align 4,,10
	.p2align 3
.L30:
	movzx	r11d, BYTE PTR [rsp+6+r10]
	sub	r10, 1
	mov	BYTE PTR [rcx+1], 7
	add	rcx, 2
	mov	BYTE PTR [rcx-2], r11b
	test	r10d, r10d
	jne	.L30
	add	edi, ebx
	mov	ebx, 1
	jmp	.L6
	.p2align 4,,10
	.p2align 3
.L24:
	mov	r10, QWORD PTR [rsp+48]
	lea	rcx, [r10+8]
	mov	QWORD PTR [rsp+48], rcx
	jmp	.L25
	.p2align 4,,10
	.p2align 3
.L14:
	jns	.L15
	mov	r12d, 1837
	lea	r11d, [r10+rbx]
	lea	r10d, [rdi+2]
	neg	ecx
	mov	WORD PTR [rbp+753664], r12w
.L15:
	mov	ebx, 1
	mov	ebp, 3435973837
	.p2align 4,,10
	.p2align 3
.L16:
	mov	edi, ecx
	imul	rdi, rbp
	shr	rdi, 35
	lea	r12d, [rdi+rdi*4]
	add	r12d, r12d
	sub	ecx, r12d
	mov	r12, rbx
	add	ecx, 48
	mov	BYTE PTR [rsp+15+rbx], cl
	mov	ecx, edi
	lea	rbx, [rbx+1]
	test	edi, edi
	jne	.L16
	lea	ecx, [r11+r11]
	movsx	rdi, r12d
	movsx	rcx, ecx
	add	rcx, 753664
	.p2align 4,,10
	.p2align 3
.L17:
	movzx	r11d, BYTE PTR [rsp+15+rdi]
	sub	rdi, 1
	mov	BYTE PTR [rcx+1], 7
	add	rcx, 2
	mov	BYTE PTR [rcx-2], r11b
	test	edi, edi
	jne	.L17
	lea	edi, [r10-1+r12]
	mov	ebx, 1
	jmp	.L6
	.p2align 4,,10
	.p2align 3
.L34:
	mov	edi, r9d
	jmp	.L2

	.local	cursor_col
	.comm	cursor_col,4,4
	.local	cursor_row
	.comm	cursor_row,4,4
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC3:
	.quad	3978425819141910832
	.quad	7378413942531504440

