/*
 * 
 * Copyright 2019 Nikita Makarevich
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

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
