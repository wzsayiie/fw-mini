cmake_minimum_required(VERSION 3.4.1)

### configuration begin.
set(lib_name main)

list(APPEND inc_path ../../appsrc)
list(APPEND src_aims ../../appsrc/*.cpp)
list(APPEND src_aims src/main/cpp/*.cpp)

list(APPEND sys_libs log)
### configutation end.

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(${inc_path})

file(GLOB_RECURSE src_files ${src_aims})
add_library(${lib_name} SHARED ${src_files})

target_link_libraries(${lib_name} ${sys_libs})
