cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(v3 VERSION 1.0 LANGUAGES CXX CUDA)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(v3 v3.cpp)
set_source_files_properties(v3.cpp PROPERTIES LANGUAGE CUDA)
set_target_properties(v3 PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
set_target_properties(v3 PROPERTIES PUBLIC_HEADER v3.h)
target_include_directories(v3 PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

install(FILES v3.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS v3 LIBRARY)

## export the library
install(
  TARGETS v3
  EXPORT v3-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/v3-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/v3
)
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/v3-config-version.cmake"
  COMPATIBILITY AnyNewerVersion)

install(FILES
  "${PROJECT_BINARY_DIR}/v3-config.cmake"
  "${PROJECT_BINARY_DIR}/v3-config-version.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/v3)

install(
  EXPORT v3-targets
  NAMESPACE v3::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/v3)
