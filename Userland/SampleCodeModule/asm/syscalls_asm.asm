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
GLOBAL mallocSyscall
GLOBAL freeSyscall
GLOBAL getPidSyscall
GLOBAL psSyscall
GLOBAL niceSyscall
GLOBAL blockSyscall
GLOBAL createProcessSyscall
GLOBAL killSyscall
GLOBAL yieldSyscall
GLOBAL exitProcessSyscall
GLOBAL sbrkSyscall

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

mallocSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 14
    int 80h

    mov rsp, rbp
    pop rbp
    ret

freeSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 15
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getPidSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 16
    int 80h

    mov rsp, rbp
    pop rbp
    ret

psSyscall:  ;printProcessList
    push rbp
    mov rbp, rsp

    mov rax, 17
    int 80h

    mov rsp, rbp
    pop rbp
    ret

niceSyscall:    ;changeProcessPriority
    push rbp
    mov rbp, rsp

    mov rax, 18
    int 80h

    mov rsp, rbp
    pop rbp
    ret

blockSyscall:    ;blockProcess
    push rbp
    mov rbp, rsp

    mov rax, 19
    int 80h

    mov rsp, rbp
    pop rbp
    ret

createProcessSyscall:    ;createProcess
    push rbp
    mov rbp, rsp

    mov rax, 20
    int 80h

    mov rsp, rbp
    pop rbp
    ret

killSyscall:    ;killProcess
    push rbp
    mov rbp, rsp

    mov rax, 21
    int 80h

    mov rsp, rbp
    pop rbp
    ret

yieldSyscall:    ;resignCPU
    push rbp
    mov rbp, rsp

    mov rax, 22
    int 80h

    mov rsp, rbp
    pop rbp
    ret

exitProcessSyscall:    ;exitProcess
    push rbp
    mov rbp, rsp

    mov rax, 23
    int 80h

    mov rsp, rbp
    pop rbp
    ret

sbrkSyscall:
  push rbp
  mov rbp, rsp

  mov rax, 24
  int 80h

  mov rsp, rbp
  pop rbp
  ret