set(SOURCE_FILES color_polygon.c)
# 编译出可执行文件
add_executable(color_polygon ${SOURCE_FILES})

# 添加依赖库
target_link_libraries(color_polygon common)
target_link_libraries(color_polygon glesRender)