    [BITS 16]
    [ORG 0x7c00]

    mov ax, 0xb800
    mov es, ax
    mov cx, 0x1000              ; 设置cx寄存器，指定loop次数
    mov bx, 0
ClearScreen:
    mov word [es:bx], 0x0720    ; 空格
    add bx, 2
    loop ClearScreen

    mov si, HelloString         ; 设置要显示字符串
    mov bx, 0
    mov cx, HelloStringLen-1    ; 设置loop循环次数
    mov ah, 0x07                ; 设置颜色
    call PrintScreen
    mov si, TestString
    mov cx, TestStringLen-1
    mov ah, 0x20                ; 设置颜色
    call PrintScreen
    jmp $
PrintScreen:
PrintLoop:
    mov al, [si]
    mov word [es:bx], ax
    add bx, 2
    inc si
    loop PrintLoop
    ret

    ;; data
    HelloString db 'Hello World!', 0
    HelloStringLen equ $-HelloString
    TestString db 'This is test.', 0
    TestStringLen equ $-TestString

    times 510 - ($-$$) db 0

    DW 0xaa55