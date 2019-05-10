
#include "nodebuilder.hpp"

namespace LSCL
{

namespace Nodebuilder
{

/**
 * Class constructor.
 * Initializes class from input stream,
 * reads first portion of characters into rx buffer
 */
Builder::Builder(std::istream &input) :
	ss(input)
{
}

} // Namespace Nodebuilder

} // Namespace LSCL
