set(SOURCE_FILES esShader.c esShapes.c esTransform.c esUtil.c)
# 编译出来的是.a文件
add_library(common ${SOURCE_FILES})

target_link_libraries(common X11)