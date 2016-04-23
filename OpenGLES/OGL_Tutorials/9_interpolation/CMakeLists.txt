add_executable(out_interpolation interpolation_out.c interpolation.c)
target_link_libraries(out_interpolation GLESv2)
target_link_libraries(out_interpolation glut)
# 使用math.h
target_link_libraries(out_interpolation m)
