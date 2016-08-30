参考[Writing Hello World Bootloader](http://viralpatel.net/taj/tutorial/hello_world_bootloader.php)

### 1. 编译
```
$ nasm fooboot.asm -f bin -o boot.bin
```

### 2. 运行
```
$ qemu-system-x86_64 -m 16 -boot c -hda ./boot.bin
```

### 3. 反编译bootloader
参考[How do I disassemble raw x86 code?](http://stackoverflow.com/questions/1737095/how-do-i-disassemble-raw-x86-code)
```
objdump -D -b binary -mi386 -Maddr16,data16 boot.bin
```

### 4. 在屏幕上显示字符
```
INT 0x10 is a BIOS video interrupt. All the video related calls are made through this interrupt.
To use this interrupt we need to set the values of some register.
AL = ASCII value of character to display
AH = 0x0E ;Teletype mode (This will tell bios that we want to print one character on screen)
BL = Text Attribute (This will be the fore ground and background color
     of character to be displayed. 0x07 in our case.)
BH = Page Number (0x00 for most of the cases)
Once all the registers all filled with appropriate value, we can call interrupt.
```

### 5. 中断0x10
参考
+ [INT 10H](https://en.wikipedia.org/wiki/INT_10H)，不同AH值对应的不同功能
+ [BIOS color attributes](https://en.wikipedia.org/wiki/BIOS_color_attributes)，BIOS的BL对应的颜色
