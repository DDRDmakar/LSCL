#
# 
# Copyright 2019 Nikita Makarevich
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# 
#


cmake_minimum_required(VERSION 3.0)

set(LSCL_VERSION_MAJOR 0)
set(LSCL_VERSION_MINOR 0)
set(LSCL_VERSION_PATCH 0)
set(LSCL_VERSION "${LSCL_VERSION_MAJOR}.${LSCL_VERSION_MINOR}.${LSCL_VERSION_PATCH}")

project(lscl
	VERSION ${LSCL_VERSION} 
	DESCRIPTION "Local Stuff Configuration Language"
	LANGUAGES CXX
)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -Os -g -pedantic -Wextra -Wall -std=c++11 -Wno-deprecated-register")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(
	lscl_parser
	src/flexbison/lscl_parser.yy
	${CMAKE_CURRENT_BINARY_DIR}/lscl_parser.tab.cc # Will be created automatically
)
FLEX_TARGET(
	lscl_lexer
	src/flexbison/lscl_lexer.l
	${CMAKE_CURRENT_BINARY_DIR}/lscl_lexer.yy.cc # Will be created automatically
)
ADD_FLEX_BISON_DEPENDENCY(lscl_lexer lscl_parser)


include(GNUInstallDirs)

set( LSCL_SOURCES
	src/cpp/nodebuilder.cpp
	src/cpp/global.cpp
	src/cpp/exception.cpp
	src/cpp/node_internal.cpp
	src/cpp/node.cpp
	src/cpp/test.cpp
)

set( LSCL_HEADERS
	lscl/lscl.hpp
	src/cpp/defines.hpp
	src/cpp/nodebuilder.hpp
	src/cpp/global.hpp
	src/cpp/exception.hpp
	src/cpp/node_internal.hpp
	src/cpp/node.hpp
	src/cpp/test.hpp
)

set( ALLSRC
	${LSCL_SOURCES}
	${LSCL_HEADERS}
	${FLEX_lscl_lexer_OUTPUTS}
	${BISON_lscl_parser_OUTPUTS}
)


# this is the "object library" target: compiles the sources only once
add_library(lscl_object OBJECT ${ALLSRC})

# shared libraries need PIC
set_property(TARGET lscl_object PROPERTY POSITION_INDEPENDENT_CODE 1)

# shared and static libraries built from the same object files
add_library(lscl_shared SHARED $<TARGET_OBJECTS:lscl_object>)
add_library(lscl_static STATIC $<TARGET_OBJECTS:lscl_object>)


include_directories(${CMAKE_SOURCE_DIR})

target_include_directories(
	lscl_object
	PRIVATE
	src/cpp
	src/flexbison
	${CMAKE_CURRENT_BINARY_DIR}
)

set_target_properties(
	lscl_object PROPERTIES
	VERSION ${PROJECT_VERSION}
	PUBLIC_HEADER lscl/lscl.hpp
	CXX_STANDARD 11
)

install(
	TARGETS lscl_shared
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

configure_file(lscl.pc.in lscl.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/lscl.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)


#enable_testing()
#add_test(
#	NAME basic
#	COMMAND ${CMAKE_SOURCE_DIR}/test/test0.pl
#	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#)

# TESTING
#option(LSCL_BUILD_TESTS "Determines whether to build tests." OFF)
#if(LSCL_BUILD_TESTS)
#	file(GLOB LSCL_SHARED_TEST_SOURCES "test/*.cpp" "test/*.hpp")
#	enable_testing()
#	find_package(Catch2 REQUIRED)
#	include(CTest)
#	include(Catch)
#	add_executable(lscl_shared_test ${LSCL_SHARED_TEST_SOURCES})
#	target_link_libraries(lscl_shared_test Catch2::Catch2)
#	#target_link_libraries(lscl_shared_test ${CMAKE_SOURCE_DIR}/build/liblscl_static.a)
#endif()
