set(SOURCE_FILES com_example_zsy_hellogles_GLES2JNILib.c)

set(directory "../../Android/HelloGLES/app/src/main/jniLibs/${ANDROID_ABI}/")

# 在编译时创建目录
add_custom_target(build-time-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${directory})

# 编译出来的是so动态库
add_library(gles2jni SHARED ${SOURCE_FILES})
# 设置编译后文件输出路径
SET(LIBRARY_OUTPUT_PATH ${directory})

target_link_libraries(gles2jni  glesRender)