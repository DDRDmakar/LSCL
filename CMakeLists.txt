
cmake_minimum_required(VERSION 3.0)

project(lscl VERSION 0.0 DESCRIPTION "Local Stuff Configuration Language")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -Os -pedantic -Wextra -Wall -std=c++11")

include(GNUInstallDirs)

add_library(
	lscl_shared SHARED
	src/lscl.cpp
)

add_library(
	lscl_static STATIC
	src/lscl.cpp
)

target_include_directories(lscl_shared PRIVATE src)
target_include_directories(lscl_static PRIVATE src)

set_target_properties(
	lscl_shared PROPERTIES
	VERSION ${PROJECT_VERSION}
	PUBLIC_HEADER lscl.hpp
)

set_target_properties(
	lscl_static PROPERTIES
	VERSION ${PROJECT_VERSION}
	PUBLIC_HEADER lscl.hpp
)

install(
	TARGETS lscl_shared
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

configure_file(lscl.pc.in lscl.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/lscl.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
