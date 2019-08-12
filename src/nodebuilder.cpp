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

#include <string>

#include "nodebuilder.hpp"
#include "exception.hpp"
#include "global.hpp"

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
	NODEWAY last = NODEWAY_MAP;
	
	ss.skip_spaces();
	
	while (ss)
	{
		const char c = ss.peek_next_char();
		last = nodestack.top();
		
		switch (c)
		{
			case '{':
			{
				nodestack.push(NODEWAY_MAP);
				break;
			}
			case '[':
			{
				nodestack.push(NODEWAY_LIST);
				nodestack.push(NODEWAY_COMMA); // Push comma such if it was comma first in the list
				break;
			}
			case ',':
			{
				if (last == NODEWAY_LIST) nodestack.push(NODEWAY_COMMA);
				else if (last != NODEWAY_COMMA) throw LSCL::Exception::Exception_nodebuilder("Values in the list are not comma-separated", ss.get_line());
				
				break;
			}
			case '#':
			{
				// process_directive()
				break;
			}
			default:
			{
				process_scalar();
			}
		}
		
		ss.eat_next_char(); // Go to next char
		ss.skip_spaces(); // Skip blank space before next trigger
		
		//else { throw LSCL::Exception::Exception_nodebuilder("Incorrect syntax", ss.get_line()); }
		
	}
}

// Processing of value in single quotes (without escape-sequences)
std::string Builder::process_scalar(void)
{
	std::string answer;
	register char c;
	c = ss.peek_next_char();
	
	bool escaped        = false;
	bool quote_single   = false;
	bool quote_double   = false;
	bool skiping_spaces = true;
	bool triangle       = false; // Triangle brackets
	
	while (c)
	{
		c = ss.pop_next_char();
		
		// Skipping blank space before value start and in the beginning
		// of each line (if it is multiline scalar value)
		if (skiping_spaces)
		{
			while (c == ' ' || c == '\t') c = ss.pop_next_char();
		}
		skiping_spaces = false;
		
		// If function meets double quote
		if (c == '\"' && !escaped && !quote_single)
		{
			if (quote_double) break;
			quote_double = true; // Change quotation status
			continue;
		}
		
		// If function meets single quote
		if (c == '\'' && !escaped && !quote_double)
		{
			if (quote_single) break;
			quote_single = true; // Change quotation status
			continue;
		}
		
		// Escaped characters
		if (escaped && !quote_single)
		{
			switch (c)
			{
				case 'n': { answer.push_back('\n'); break; } // Line-break character
				case 't': { answer.push_back('\t'); break; } // Tabulator character
				case 's': { answer.push_back(' ');  break; } // Space
				case 'r': { answer.push_back('\r'); break; } // Carriage return
				case 'x': { // Hex representation
					std::string current_hex_value;
					c = ss.peek_next_char();
					// Pushing hex symbol code into string
					while (
						('0' <= c && c <= '9') ||
						('a' <= c && c <= 'f') ||
						('A' <= c && c <= 'F')
					)
					{
						current_hex_value.push_back(ss.pop_next_char());
						c = *(current_hex_value.end() - 1);
					}
					
					if (c == ';') // If ending symbol is valid (; in the end if hex symbol surrogate)
					{
						// Convert hex string into symbol (maybe )
						
						current_hex_value.pop_back();
						std::vector<uint32_t> surrogate_vector = { (uint32_t)strtoul(current_hex_value.c_str(), NULL, 16) };
						current_hex_value = uint32_vector_to_string(surrogate_vector);
						for (char e : current_hex_value)
						{
							answer.push_back(e);
						}
					}
					// Else throw exception
					else throw LSCL::Exception::Exception_nodebuilder("Symbol hex code in string is defined incorrectly. Correct format is: \\x< hex code >;", ss.get_line());
					
					break;
				}
				// In all other cases just save escaped character
				default:
				{
					answer.push_back(c);
					break;
				}
			}
			
			escaped = false; // End escape
			continue;
		}
		
		// If function meets line-break
		if (!escaped && c == '\n')
		{
			// If line-break is inside quotes
			if (quote_double || quote_single)
			{
				// \n
				if (triangle) answer.push_back('\n');
				skiping_spaces = true;
				continue;
			}
			// If line-break is beyond quotes
			else break;
		}
		
		// If function meets escaping character
		// If it's not escaped, then start escape
		if (c == '\\' && !escaped && !quote_single) { escaped = true; continue; }
		// Else we have to end escape (tell, that it's normal symbol, need it to work with "\" symbol)
		// And other symbols, which shouldn`t be escaped
		else escaped = false;
		
		// ==== End of escaped characters processing, under it 'ESCAPED' always FALSE
		
		// Push current character into collector
		answer.push_back(c);
	}
	
	return answer;
}

} // Namespace Nodebuilder

} // Namespace LSCL
