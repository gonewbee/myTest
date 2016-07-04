; 编译: nasm -f elf64 barf.s && ld -s -o barf barf.o
section .data
    fname db 'test.txt', 0          ; 加0，可用来表示字符串结尾
    hello db 'Hello asm world', 10  ; ‘Hello asm world’加一个换行符(10)
    helloLen equ $-hello            ; 字符串长度

section .bss
    fd_out resb 1   ; 在.bss段中声明变量，用来保存打开的文件句柄

section .text
    global _start

_start:
    ; 创建文件
    mov eax, 8 
    mov ebx, fname
    mov ecx, 511        ; 文件权限
    int 0x80

    mov [fd_out], eax   ; 将返回值保存

    mov eax,4           ; 系统调用sys_write
    mov ebx,[fd_out]    ; 文件描述符，创建的文件
    mov ecx,hello       ; 设置字符串偏移地址到ecx
    mov edx,helloLen    ; 设置字符串长度
    int 80h             ; 中断0x80, kernel系统调用syscall

    ; 关闭文件
    mov eax, 6
    mov ebx, [fd_out]
    int 0x80

    mov eax,1           ; exit的系统调用(sys_exit)
    mov ebx,0           ; 退出值我为0
    int 80h
