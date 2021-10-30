GLOBAL cpuVendor
GLOBAL RTC
GLOBAL _xchg

section .text

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

RTC:
	; push rbp
	; mov rbp, rsp
	;
	; mov rax, rdi
	; out 70h, al
	; in al, 71h
	;
	; mov rsp, rbp
	; pop rbp
	; ret
	push rbp
	mov rbp, rsp

	push rbx
	push rcx

	mov rcx, 10
	mov rax, rdi
	out 70h, al
	in al, 71h
	mov bx, ax
	and ax, 0xF0
	and bx, 0x0F
	shr rax, 4
	mul rcx
	add ax, bx

	pop rcx
	pop rbx

	mov rsp, rbp
	pop rbp
	ret


_xchg:
	mov rax, rsi
	xchg [rdi], eax
	ret