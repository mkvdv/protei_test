cmake_minimum_required(VERSION 3.8)
project(protei_test)

find_package(Boost COMPONENTS program_options REQUIRED)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(client
        src/client/main.cpp
        src/client/tcp_client.cpp
        src/client/udp_client.cpp
        src/sockets/tcp_socket.cpp
        src/sockets/udp_socket.cpp
        src/sockets/check_errors.cpp
        )

add_executable(server
        src/server/main.cpp
        src/server/tcp_server.cpp
        src/server/udp_server.cpp
        src/server/response_processor.cpp
        src/sockets/tcp_socket.cpp
        src/sockets/udp_socket.cpp
        src/sockets/check_errors.cpp
        )

message("${CMAKE_CXX_COMPILER_ID}")
macro(set_warning_flags target std_ver)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message("!!! GCC DETECTED !!!!")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD ${std_ver}
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "-O2;-Wall;-Wextra;-Werror;-Wpedantic;-ggdb"
                )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("!!! CLANG DETECTED !!!!")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD ${std_ver}
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "-O2;-Wall;-Wextra;-Werror;-pedantic"
                )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message("!!! MSVC DETECTED !!!!")
        set_target_properties(${target} PROPERTIES
                CXX_STANDARD ${std_ver}
                CXX_STANDARD_REQUIRED ON
                COMPILE_OPTIONS "/W4"
                )
    else ()
        message(FATAL_ERROR "!!! NO COMPILER DETECTED, WARNING FLAGS NOT SETTED !!!!")
    endif ()
endmacro()


set_warning_flags(server 14)
target_link_libraries(server
        ${Boost_LIBRARIES}
        )

set_warning_flags(client 14)
target_link_libraries(client
        ${Boost_LIBRARIES}
        )
