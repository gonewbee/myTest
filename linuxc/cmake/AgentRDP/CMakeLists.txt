cmake_minimum_required(VERSION 2.8.4)
project(AgentRDP)

set(AGENTRDP_VERSION_MAJOR "0")
set(AGENTRDP_VERSION_MINOR "1")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

add_subdirectory(agent)
add_subdirectory(include)
