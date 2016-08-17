### 1. qemu-img将vhd转换为raw
```sh
qemu-img convert -f vpc -O raw vhd2.vhd vhd2.raw
```
### 2. qemu使用raw格式镜像
```sh
qemu-system-x86_64 -drive format=raw,file=vhd2.raw
```

