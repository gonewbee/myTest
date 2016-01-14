;
; assemble and link with:
; nasm -f elf64 printf-test.asm && gcc printf-test.o
;
; http://stackoverflow.com/questions/8194141/how-to-print-a-number-in-assembly-nasm/32853546#32853546
;
extern printf, exit
section .data
    format db "%x", 10, 0
section .text
    global main
    main:
        sub rsp, 8
        mov rsi, 0x12345678
        mov rdi, format
        xor rax, rax
        call printf
        mov rdi, 0
        call exit