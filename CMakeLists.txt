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

#option(BUILD_DOC "Build documentation" ON)
option(BUILD_DOC "Build documentation" OFF)

project(lscl
	VERSION ${LSCL_VERSION} 
	DESCRIPTION "Local Stuff Configuration Language"
	LANGUAGES CXX
)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lm -Os -g -pedantic -Wextra -Wall -std=c++11 -Wno-deprecated-register")

find_package(Threads REQUIRED)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(Doxygen)

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
	src/cpp/script.cpp
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
	src/cpp/script.hpp
)

set( ALLSRC
	${LSCL_SOURCES}
	${LSCL_HEADERS}
	${FLEX_lscl_lexer_OUTPUTS}
	${BISON_lscl_parser_OUTPUTS}
)


# this is the "object library" target: compiles the sources only once
add_library(lscl_object OBJECT ${ALLSRC})

# -l pthread
#target_link_libraries(lscl_object, ${CMAKE_THREAD_LIBS_INIT})

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

target_link_libraries(
	lscl_object pthread
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

if (BUILD_DOC)
	if (DOXYGEN_FOUND)
		# set input and output files
		set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
		set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

		# request to configure the file
		configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
		message("Doxygen build started")

		# note the option ALL which allows to build the docs together with the application
		add_custom_target( doc_doxygen ALL
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen"
			VERBATIM )
	else (DOXYGEN_FOUND)
	message("Doxygen need to be installed to generate the doxygen documentation")
	endif (DOXYGEN_FOUND)
endif (BUILD_DOC)

