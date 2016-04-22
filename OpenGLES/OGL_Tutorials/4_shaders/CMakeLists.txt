add_executable(out_shaders shaders_out.c shaders.c)
target_link_libraries(out_shaders GLESv2)
target_link_libraries(out_shaders glut)
