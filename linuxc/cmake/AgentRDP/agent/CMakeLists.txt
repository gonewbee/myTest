set(SOURCE_FILES main.c agent_client.c agent_client.h agentrdp.h encoder.c encoder.h)

# 设置头文件路径
include_directories(../include)

LINK_DIRECTORIES(/home/zsy/Workspace/down/FreeRDP/out/usr/local/lib/x86_64-linux-gnu)

add_executable(AgentRDP ${SOURCE_FILES})
target_link_libraries(AgentRDP freerdp-client)
target_link_libraries(AgentRDP freerdp)
target_link_libraries(AgentRDP winpr)
target_link_libraries(AgentRDP avcodec)
target_link_libraries(AgentRDP avutil)
target_link_libraries(AgentRDP swscale)
