
cmake_minimum_required(VERSION 3.0)

set(LSCL_VERSION_MAJOR 0)
set(LSCL_VERSION_MINOR 0)
set(LSCL_VERSION_PATCH 0)
set(LSCL_VERSION "${LSCL_VERSION_MAJOR}.${LSCL_VERSION_MINOR}.${LSCL_VERSION_PATCH}")

project(lscl 
	VERSION ${LSCL_VERSION} 
	DESCRIPTION "Local Stuff Configuration Language"
)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -Os -pedantic -Wextra -Wall -std=c++11")

include(GNUInstallDirs)

file(GLOB LSCLSRC "src/*.cpp")

include_directories(${CMAKE_SOURCE_DIR})

add_library(
	lscl_shared SHARED
	${LSCLSRC}
)

add_library(
	lscl_static STATIC
	${LSCLSRC}
)

target_include_directories(lscl_shared PRIVATE src)
target_include_directories(lscl_static PRIVATE src)

set_target_properties(
	lscl_shared PROPERTIES
	VERSION ${PROJECT_VERSION}
	PUBLIC_HEADER lscl/lscl.hpp
)

set_target_properties(
	lscl_static PROPERTIES
	VERSION ${PROJECT_VERSION}
	PUBLIC_HEADER lscl/lscl.hpp
)

add_subdirectory(test)

install(
	TARGETS lscl_shared
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

configure_file(lscl.pc.in lscl.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/lscl.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
