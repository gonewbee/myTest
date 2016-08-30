### 1. 编译
```
$ nasm fooboot.asm -f bin -o boot.bin
```

### 2. 运行
```
$ qemu-system-x86_64 -m 16 -boot c -hda ./boot.bin
```

