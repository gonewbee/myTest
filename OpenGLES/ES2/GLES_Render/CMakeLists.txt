set(SOURCE_FILES gles2_triangle.c gles2_polygon.c gles2_render.c)

add_library(glesRender ${SOURCE_FILES})

target_link_libraries(glesRender GLESv2)
target_link_libraries(glesRender EGL)
