### clion将cpp工程修改为c工程
参考[How to create a C project with CLion](http://stackoverflow.com/questions/26177390/how-to-create-a-c-project-with-clion)修改CMakeLists.txt，将

```sh
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
```
替换为

```sh
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
```
然后重命名main.cpp为main.c

### 通过.in配置生成对于文件
在CMakeLists.txt中使用configure_file进行配置

### 使用第三方动态库

```sh
LINK_DIRECTORIES(/home/zsy/Workspace/down/FreeRDP/out/usr/local/lib/x86_64-linux-gnu)

add_executable(AgentRDP ${SOURCE_FILES})
target_link_libraries(AgentRDP freerdp-client)
```

