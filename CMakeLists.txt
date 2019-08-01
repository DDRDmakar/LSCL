
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

set(LSCL_SOURCES
	src/lscl.cpp
	src/nodebuilder.cpp
	src/scalar.cpp
	src/stream.cpp
	src/global.cpp
	src/exception.cpp
	src/node_internal.cpp
)

set(LSCL_HEADERS
	src/defines.hpp
	src/nodebuilder.hpp
	src/scalar.hpp
	src/stream.hpp
	src/exception.hpp
	src/node_internal.hpp
)

include_directories(${CMAKE_SOURCE_DIR})

add_library(
	lscl_shared SHARED
	"${LSCL_SOURCES};${LSCL_HEADERS}"
)

add_library(
	lscl_static STATIC
	${LSCL_SOURCES} ${LSCL_HEADERS}
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

# TESTING
option(LSCL_BUILD_TESTS "Determines whether to build tests." OFF)
if(LSCL_BUILD_TESTS)
	file(GLOB LSCL_SHARED_TEST_SOURCES "test/*.cpp" "test/*.hpp")
	enable_testing()
	find_package(Catch2 REQUIRED)
	include(CTest)
	include(Catch)
	add_executable(lscl_shared_test ${LSCL_SHARED_TEST_SOURCES})
	target_link_libraries(lscl_shared_test Catch2::Catch2)
	#target_link_libraries(lscl_shared_test ${CMAKE_SOURCE_DIR}/build/liblscl_static.a)
endif()

install(
	TARGETS lscl_shared
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

configure_file(lscl.pc.in lscl.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/lscl.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
