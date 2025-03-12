cmake_minimum_required(VERSION 3.20)

project(tcp_packets)
set(CMAKE_C_STANDARD 17)

set(HEADER_FILES
    ${CMAKE_SOURCE_DIR}/src/card.h
)

if (WIN32)
    target_link_libraries(server wsock32 ws2_32)
    target_link_libraries(client wsock32 ws2_32)
endif()

add_executable(server ${HEADER_FILES} ${CMAKE_SOURCE_DIR}/src/server.c)
add_executable(client ${HEADER_FILES} ${CMAKE_SOURCE_DIR}/src/client.c)