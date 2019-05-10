
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(lscl_shared_test test1.cpp)

enable_testing()
find_package(Catch2 REQUIRED)
include(CTest)
include(Catch)
target_link_libraries(lscl_shared_test Catch2::Catch2)

#add_test()

#include(ParseAndAddCatchTests)
#ParseAndAddCatchTests(lscl_shared_test)

#catch_discover_tests(lscl_shared)

#set(TEST_TIMEOUT 6)
#enable_testing()
#add_test(Stream_FromSimpleString_CharSequence)
#set_tests_properties(
#	Stream_FromSimpleString_CharSequence 
#	PROPERTIES TIMEOUT ${MULTIPLY_TIMEOUT}
#)
