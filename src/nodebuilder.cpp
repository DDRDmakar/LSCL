
#include "nodebuilder.hpp"
#include "exception.hpp"

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
	NODEWAY last = MAP;
	
	ss.skip_spaces();
	
	while (ss)
	{
		const char c = ss.pop_next_char();
		last = nodestack.top();
		
		if (c == '{')
		{
			nodestack.push(MAP);
		}
		else if (c == '[')
		{
			nodestack.push(LIST);
			nodestack.push(COMMA); // Push comma such if it was comma first in the list
		}
		else if (c == ',')
		{
			if (last == LIST) nodestack.push(COMMA);
			else if (last != COMMA) throw LSCL::Exception::Exception_nodebuilder("Values in the list are not comma-separated", ss.get_line());
		}
		else if (c == '\'')
		{
			//process_quotes_single();
		}
		else if (c == '\"')
		{
			//process_quotes_double();
		}
		else if (c == '#')
		{
			//process_directive();
		}
		
		else if (last == SCALAR)
		{
			// Process simple scalar (flat text)
			//process_scalar_simple();
		}
		
		else { throw LSCL::Exception::Exception_nodebuilder("Incorrect syntax", ss.get_line()); }
		
		ss.skip_spaces(); // Skip blank space before next trigger
	}
}

} // Namespace Nodebuilder

} // Namespace LSCL
