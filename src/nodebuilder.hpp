
#ifndef _H_LSCL_NODEBUILDER
#define _H_LSCL_NODEBUILDER

#include <string>
#include <sstream>
#include <stack>

#include "../lscl/lscl.hpp"
#include "stream.hpp"

using namespace std;

namespace LSCL
{
	
	namespace Nodebuilder
	{
		
		/*
		* It builds a tree of nodes from text stream
		* 
		*/
		class Builder
		{
		public:
			
			explicit Builder(std::istream& input);
			
			// If the parser has some valid input to be read
			// explicit operator bool() const;
			
		private:
			
			enum NODEWAY
			{
				MAP, LIST, SCALAR, KEY, COMMA
			};
			
			Stream ss;                     // Characters stream to read from
			std::stack<NODEWAY> nodestack; // Stack of the hierarchy we are digging into
			
		};
		
	} // Namespace Nodebuilder

} // Namespace LSCL

#endif // _H_LSCL_NODEBUILDER
