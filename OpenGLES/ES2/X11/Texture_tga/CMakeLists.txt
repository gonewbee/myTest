set(SOURCE_FILES texture_tga.c)
# 编译出可执行文件
add_executable(texture_tga ${SOURCE_FILES})

# 添加依赖库
target_link_libraries(texture_tga common)
target_link_libraries(texture_tga glesRender)
