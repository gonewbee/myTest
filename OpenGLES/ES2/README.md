ubuntu安装OpenGL ES 2.0

```sh
$ sudo apt-get install libgles2-mesa-dev
```

## 编译

### Android

```sh
cmake -DCMAKE_TOOLCHAIN_FILE=cmake/android.toolchain.cmake \
    -DANDROID_NDK=/home/zsy/Tools/android/android-ndk-r10e \
    -DCMAKE_BUILD_TYPE=Release \
    -DANDROID_NATIVE_API_LEVEL=android-18 \
    -DANDROID_ABI="armeabi-v7a" .
```
