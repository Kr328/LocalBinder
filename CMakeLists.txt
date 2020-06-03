cmake_minimum_required(VERSION 2.8)
project(local_binder)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(lbinder SHARED executor.cpp executor.h parcel.cpp parcel.h buffer_pool.cpp buffer_pool.h binder.cpp binder.h binder_bus.cpp binder_bus.h)
target_link_libraries(lbinder pthread)
target_include_directories(lbinder PUBLIC .)