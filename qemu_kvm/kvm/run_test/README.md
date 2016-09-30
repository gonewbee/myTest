+ asm文件编译方式: nasm -f bin -o loop.out loop.asm
+ 将编译后内容用0x的方式输出: python b2txt.py loop.out
+ 反编译: $ objdump -D -b binary -mi386 -Maddr16,data16 loop.out