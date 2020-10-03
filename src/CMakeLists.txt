cmake_minimum_required (VERSION 3.16)

set(sources main.cpp Socket.h Socket.cpp)

add_executable (SimpHTTP ${sources})
target_link_libraries(SimpHTTP spdlog::spdlog)

if (WIN32)
	target_link_libraries(SimpHTTP Ws2_32)
endif()
