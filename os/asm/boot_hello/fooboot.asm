    [BITS 16]
    [ORG 0x7c00]

    MOV AL, 65
    CALL PrintCharacter
    JMP $
    TIMES 510 - ($-$$) db 0
    DW 0xAA55
    