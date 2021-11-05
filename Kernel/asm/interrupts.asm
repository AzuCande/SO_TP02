GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq80Handler
GLOBAL _exception00Handler
GLOBAL _exception06Handler
GLOBAL saveInitialConditions
GLOBAL _sendEOI
GLOBAL _hlt
GLOBAL createProcessInt
GLOBAL goToProcess

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN scheduler

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rsi, rsp ; pasaje del "vector" de registros
	mov rdi, %1 ; pasaje de parametro

	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	; mov al, 20h
	; out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	mov rsi, rsp ; pasaje de registros
	call exceptionDispatcher

	mov rax, [initialConditions] ; rsp
	mov [rsp + 18*8], rax
	mov rax, [initialConditions + 8] ; rip
	mov [rsp + 15*8], rax

	popState
	iretq
%endmacro

_cli:
    cli
    ret

_sti:
    sti
    ret

picMasterMask:
    push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
		mov rsp, rbp
    pop rbp
    retn

picSlaveMask:
    push rbp
    mov rbp, rsp
    mov ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
		mov rsp, rbp
    pop rbp
    retn

; Timer Tick
_irq00Handler:
    pushState

	mov rsi, rsp	; pasaje de parametro
	mov rdi, 0	; pasaje de parametro
	call irqDispatcher

	mov rdi, rsp
	call scheduler
	mov rsp, rax

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq

; Keyboard
_irq01Handler:
    irqHandlerMaster 1

_irq80Handler:
    irqHandlerMaster 80

_exception00Handler:
	exceptionHandler 0

_exception06Handler:
	exceptionHandler 6

saveInitialConditions:
	mov rax, rsp
	mov [initialConditions], rax
	mov rax, rdi
	mov [initialConditions + 8], rax
	ret

_hlt:
	hlt
	ret

_sendEOI:
	push rax
	mov al, 20h
	out 20h, al
	pop rax
	ret


createProcessInt:
	;pusheo el bp y el sp del proceso que venia corriento antes
	mov r8, rbp
	mov r9, rsp
	;cambio el stack al stack del proceso
	mov rbp, rsi ; apunto los SP al final de la memoria 
	mov rsp, rsi			;es decir comienzo de stack
	push 0x0 ;Ss = 0x0
	push rbp ; rsp apuntar al final de la memoria pedida
	push 0x202 ; rflags = 0x202
	push 0x8 ; cs, 0x8
	push rdi ; rpi, main
	;registers
	push 0
	push 1
	push 2
	push 3
	push 4
	push rdx ; rdi
	push rcx ; rsi
	push 7
	push 8
	push 9
	push 10
	push 11
	push 12
	push 13
	push 14
	mov rax, rsp
	;popeo memoria del proceso anterior
	mov rsp, r9
	mov rbp, r8
	ret
	
goToProcess:
	mov rsp, rdi
	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq


SECTION .bss
initialConditions resb 16 ; rsp: primeros 8 bits - rip: segundos 8 bits
