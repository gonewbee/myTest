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
+ [BIOS interrupt call](https://en.wikipedia.org/wiki/BIOS_interrupt_call)

### 6. 显存地址0xb8000
对应vramboot.asm，参考
+ [How to write to screen with video memory address 0xb8000 from real mode?](http://stackoverflow.com/questions/33681795/how-to-write-to-screen-with-video-memory-address-0xb8000-from-real-mode)
+ [x86 assembly directly write to VGA simple OS](http://stackoverflow.com/questions/18550598/x86-assembly-directly-write-to-vga-simple-os)
+ [hurlex <四> 字符模式下的显卡驱动](http://wiki.0xffffff.org/posts/hurlex-4.html)

### 7. 使用qemu的gdb和monitor功能进行调试
参考[How to step over interrupt calls when debugging a bootloader/bios with gdb and QEMU?](http://stackoverflow.com/questions/24491516/how-to-step-over-interrupt-calls-when-debugging-a-bootloader-bios-with-gdb-and-q)

#### 启动qemu
```
$ qemu-system-x86_64 -monitor stdio -s -S -m 16 -boot c -hda ./vramboot_helloworld.bin
```
+ -monitor stdio: qemu的monitor功能，可使用info registers和x /i $eip命令
+ -s: 会启动gdbserver功能，端口1234

#### gdb
启动gdb，连接localhost:1234端口
```
$ gdb
(gdb) target remote localhost:1234
(gdb) set architecture i8086
(gdb) br *0x7c00
Breakpoint 1 at 0x7c00
(gdb) c
Continuing.

Breakpoint 1, 0x00007c00 in ?? ()
(gdb) si
0x00007c03 in ?? ()
```
+ si是stepi的简写，单步跟踪一条机器指令

#### qemu monitor查看
+ x /i $eip: 显示指令
+ info registers: 查看各个寄存器状态

