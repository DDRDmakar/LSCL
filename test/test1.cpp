// 010-TestCase.cpp

// Let Catch provide main():
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <iostream>

using namespace std;

int Factorial( int number ) {
   return number <= 1 ? number : Factorial( number - 1 ) * number;  // fail
// return number <= 1 ? 1      : Factorial( number - 1 ) * number;  // pass
}

TEST_CASE( "Factorial of 0 is 1 (fail)", "[nodebuilder]" ) {
	std::cout << "kok" << std::endl;
    REQUIRE( 2 == 1 );
}

TEST_CASE( "Factorials of 1 and higher are computed (pass)", "[nodebuilder]" ) {
	std::cout << "kek" << std::endl;
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}
