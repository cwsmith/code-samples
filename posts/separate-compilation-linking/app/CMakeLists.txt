cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(main LANGUAGES CXX CUDA)

find_package(v3 REQUIRED)

add_executable(main particle.cpp main.cpp)
set_source_files_properties(main.cpp particle.cpp PROPERTIES LANGUAGE CUDA)
#set_target_properties(deviceLib PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_link_libraries(main PRIVATE v3::v3)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
