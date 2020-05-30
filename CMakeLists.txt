cmake_minimum_required(VERSION 3.16)
project(local_binder)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_library(local_binder SHARED executor.cpp executor.h parcel.cpp parcel.h buffer_pool.cpp buffer_pool.h)
target_link_libraries(local_binder pthread)