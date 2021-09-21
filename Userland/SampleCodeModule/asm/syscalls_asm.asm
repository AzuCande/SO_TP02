GLOBAL drawStringSysCall
GLOBAL readKeyboardSysCall
GLOBAL isKeyboardEmptySyscall
GLOBAL drawRectSyscall
GLOBAL clearDisplaySyscall
GLOBAL setFunctionKeyMethodSyscall
GLOBAL getTimeSyscall
GLOBAL getTicksSyscall
GLOBAL getRegistersSyscall
GLOBAL getMemSyscall
GLOBAL readErrorSyscall

section .text



drawStringSysCall:
    push rbp
    mov rbp, rsp

    mov rax, 1 ;ID write str
    mov r10, [rbp+2*8] ;sexto argumento
    mov r11, [rbp+3*8] ;septimo argumento

    int 80h

    mov rsp, rbp
    pop rbp
    ret

readKeyboardSysCall:
    push rbp
    mov rbp, rsp

    mov rax, 0    ;ID read
    int 80h

    mov rsp, rbp
    pop rbp
    ret

isKeyboardEmptySyscall:
    push rbp
    mov rbp, rsp

    mov rax, 11    ;ID bufferEmpty
    int 80h

    mov rsp, rbp
    pop rbp
    ret

clearDisplaySyscall:
    push rbp
    mov rbp, rsp

    mov rax, 2    ;ID clear Display
    int 80h

    mov rsp, rbp
    pop rbp
    ret

drawRectSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 5 ; drawRectangle syscall
    int 80h

    mov rsp, rbp
    pop rbp
    ret

setFunctionKeyMethodSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 13; setFunctionKeyMethod syscall
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getTimeSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 8
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getTicksSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 7
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getRegistersSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 9
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getMemSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 10
    int 80h

    mov rsp, rbp
    pop rbp
    ret

readErrorSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 12 ;ID readError
    int 80h

    mov rsp, rbp
    pop rbp
    ret
