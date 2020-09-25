cmake_minimum_required (VERSION 3.16)

set(sources "main.cpp" "Socket.cpp" "Socket.h" "Logger.h")

add_executable (SimpHTTP sources)

if (WIN32)
	target_link_libraries(SimpHTTP Ws2_32)
endif()