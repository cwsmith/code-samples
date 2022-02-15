cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(main LANGUAGES CXX CUDA)

find_package(v3 REQUIRED)

add_executable(main main.cpp)
set_source_files_properties(main.cpp PROPERTIES LANGUAGE CUDA)
set_target_properties(main PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_link_libraries(main PRIVATE v3::v3)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(mainNoDevCall mainNoDevCall.cpp)
set_source_files_properties(mainNoDevCall.cpp PROPERTIES LANGUAGE CUDA)
set_property(TARGET mainNoDevCall PROPERTY CUDA_RESOLVE_DEVICE_SYMBOLS ON)
target_link_libraries(mainNoDevCall PRIVATE v3::v3)
target_include_directories(mainNoDevCall PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
