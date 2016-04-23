add_executable(out_indexed_draws indexed_draws_out.c indexed_draws.c)
target_link_libraries(out_indexed_draws GLESv2)
target_link_libraries(out_indexed_draws glut)
# 使用math.h
target_link_libraries(out_indexed_draws m)
