1. 编译，先设置后ndk的环境变量，我的电脑为：
    export PATH=/opt/android-ndk-r9c/:$PATH
    执行 . build 编译
2. 将so文件往模拟器/system/lib下push遇到的两个问题：
    a. Read-only file system
        解决：mount -o remount rw /system
    b. Out of memory
        启动模拟器时指定rom大小：emulator -avd test -partition-size 256
3. 测试：执行./texampleServer和./texampleClient测试
