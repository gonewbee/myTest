    [BITS 16]
    [ORG 0x7C00]

    MOV SI, HelloString         ; 将字符串指针存到SI
    CALL PrintString            ; 调用打印字符串函数
    JMP $                       ; 死循环，程序停在这里

PrintCharacter:
    MOV AH, 0x09                ; 使用AH=0x09
    MOV BH, 0x00
    MOV BL, 0x05                ; 设置颜色Magenta
    MOV CX, 1                   ; 打印字符次数

    INT 0x10
    RET

PrintString:                    ; 在屏幕上打印字符串，假定字符串启始指针存在寄存器SI中
next_character:
    MOV AL, [SI]                ; 从字符串中获取一个字节，并存到寄存器AL中
    INC SI                      ; 自增SI指针
    OR AL, AL                   ; 判断AL中的值是否为0（字符串结尾）
    JZ exit_function            ; 如果AL值为0,退出函数
    CALL PrintCharacter         ; 否则打印AL寄存器中的字符
    CALL inc_cursor
    JMP next_character          ; 获取字符串中的下一个字符

exit_function:
    RET

inc_cursor:                     ; 将光标位置移动一位
    MOV AH, 0x03                ; AH=0x03获取光标位置和形状
    INT 0x10
    INC DL                      ; 将列位置自增1
    MOV AH, 0x02                ; AH=0x02设置光标位置
    INT 0x10
    RET

    ;; Data
    HelloString db 'Hello World', 0 ; HelloWorld字符串，以0结尾

    TIMES 510 - ($-$$) db 0     ; 剩下的空间填充0
    DW 0xAA55