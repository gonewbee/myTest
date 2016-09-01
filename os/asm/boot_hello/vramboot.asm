    [BITS 16]                   ; 告诉汇编器代码为16 bit code
    [ORG 0x7c00]                ; 起点，告诉汇编器当代码被加载后在内存中地址

    mov ax, 0xb800              ; 0xb8000是显存地址
    mov es, ax                  ; 将0xb800存到es
    mov bx, 0x0000
    mov ax, 0x2040
    mov word [es:bx], ax        ; 要显示的字符
    add bx, 2
    mov ah, 0x20                ; 背景色绿色，前景色黑色
    mov al, 0x41                ; 字符A
    mov word [es:bx], ax
    add bx, 2
    mov al, 0x42
    mov word [es:bx], ax
    JMP $                       ; 死循环，程序挂在这里

    TIMES 510 - ($-$$) db 0     ; 剩下区域用0填充, bootloader的长度为512字节，除去标志位0xAA55还
                                ; 有510字节，$表示指令的位置，$$表示程序的开始，所以($-$$)为程序的长度 
    DW 0xAA55                   ; bootloader的标志位
    