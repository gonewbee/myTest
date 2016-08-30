    [BITS 16]                   ; 告诉汇编器代码为16 bit code
    [ORG 0x7c00]                ; 起点，告诉汇编器当代码被加载后在内存中地址

    MOV AL, 65
    CALL PrintCharacter
    JMP $                       ; 死循环，程序挂在这里

PrintCharacter:                 ; 函数在屏幕上打印字符，假设ASCII字符在AL寄存器中
    MOV AH, 0x0E                ; 告诉BIOS，我们要在屏幕上打印一个字符
    MOV BH, 0x00                ; Page no
    MOV BL, 0x07                ; 文字属性0x07是黑色背景，浅灰色文字

    INT 0x10                    ; 调用veido中断
    RET                         ; 返回调用函数
    
    TIMES 510 - ($-$$) db 0     ; 剩下区域用0填充, bootloader的长度为512字节，除去标志位0xAA55还
                                ; 有510字节，$表示指令的位置，$$表示程序的开始，所以($-$$)为程序的长度 
    DW 0xAA55                   ; bootloader的标志位
    