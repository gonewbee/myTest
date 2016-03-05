set(SOURCE_FILES esShader.c esShapes.c esTransform.c esUtil.c)
# 编译出来的是so动态库
add_library(common SHARED ${SOURCE_FILES})

target_link_libraries(common X11)