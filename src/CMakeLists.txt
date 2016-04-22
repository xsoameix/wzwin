# thread
find_package(Threads REQUIRED)
set(LIBS ${LIBS} ${CMAKE_THREAD_LIBS_INIT})

# SDL
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
set(LIBS ${LIBS} ${SDL2_LIBRARIES})

# wz
find_package(WZ REQUIRED)
include_directories(${WZ_INCLUDE_DIR})
link_directories(${WZ_LIBRARY_DIRS})
set(LIBS ${LIBS} ${WZ_LIBRARIES})

# flags
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_C_COMPILER_ID}" MATCHES  "Clang")
  set(FLAGS
    "-pthread -pedantic -g"
    "-Wall -Wextra -Werror -Wconversion -Wshadow -Wwrite-strings")
  set(POSIX_FLAGS ${FLAGS} "-std=c99 -D _POSIX_C_SOURCE=200809L")
  set(C99_FLAGS ${FLAGS} "-std=c99")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
  set(FLAGS
    "/wd4668" # C++: fix __cplusplus undefined bug
    "/wd4255" # C++: fix __slwpcb() bug
    "/wd4204" # C99
    "/wd4710" # C89: inline function
    "/wd4820" # C89: struct padding
    "/wd4996" # C89: sprintf & fopen
    "/Wall")
  set(POSIX_FLAGS ${FLAGS})
  set(C99_FLAGS ${FLAGS})
endif()
string(REPLACE " " ";" POSIX_TARGET_FLAGS "${POSIX_FLAGS}")
string(REPLACE " " ";" C99_TARGET_FLAGS "${C99_FLAGS}")

# net
if (WIN32)
  set(NET net/win.c)
  set(NET_LIBS WS2_32)
else()
  set(NET net/unix.c)
endif()

# server - client program
add_executable(server
  ${NET}
  server.c)
target_compile_options(server PRIVATE ${POSIX_TARGET_FLAGS})
target_link_libraries(server ${NET_LIBS})

# client - client program
add_executable(client
  ${NET}
  client.c)
target_compile_options(client PRIVATE ${POSIX_TARGET_FLAGS})
target_link_libraries(client ${NET_LIBS})

## main - main program
add_executable(main main.c)
target_link_libraries(main ${LIBS})
target_compile_options(main PRIVATE ${C99_TARGET_FLAGS})

# main - programs
install(TARGETS main
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
if (WIN32)
  string(REPLACE "\\" "/" ZLIB_ROOT ${ZLIB_ROOT})
  string(REPLACE "\\" "/" WZ_ROOT_DIR ${WZ_ROOT_DIR})
  install(FILES
    ${ZLIB_ROOT}/bin/zlibd.dll
    ${WZ_ROOT_DIR}/bin/wz/wz.dll
    ${WZ_ROOT_DIR}/bin/wz/aes256.dll
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()
