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

#define __DEBUG

#ifdef __DEBUG
#include <iostream>
#endif

#include <string>

#include "nodebuilder.hpp"
#include "exception.hpp"
#include "global.hpp"


// Inserting new empty container to the given workpoint
#define INSERT_CONTAINER(t, t_name)            \
	if (last == NODEWAY_COMMA_MAP)             \
	{                                          \
		workpoint->values_map.insert(          \
			{                                  \
				key,                           \
				Node_internal((t), workpoint)  \
			}                                  \
		);                                               \
		auto inserted = workpoint->values_map.find(key); \
		if (inserted == workpoint->values_map.end())     \
			throw LSCL::Exception::Exception_nodebuilder("Adding " + std::string(t_name) + " into map", ss_.get_line()); \
		workpoint = &(inserted->second);                 \
	}                                                    \
	else if (last == NODEWAY_COMMA_LIST)                 \
	{                                                    \
		workpoint->values_list.push_back(                \
			Node_internal((t), workpoint)                \
		);                                               \
		workpoint = &( workpoint->values_list.back() );  \
	}                                                    \
	else throw LSCL::Exception::Exception_nodebuilder("Adding " + std::string(t_name) + " into non-container node", ss_.get_line());


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
	ss_(input),
	root(NODETYPE_MAP, nullptr)
{
#ifdef __DEBUG
	std::cout << "Nodebuilder constructor start" << std::endl;
#endif
	
	std::string key; // Key for map
	
	ss_.skip_spaces(); // Go to first non-space symbol
	
	// Detecting root type (list or map)
	const char first_c = ss_.peek_next_char(); // Get first char from stream
	if (first_c == EOF || first_c == '\0') // If no structure in file
	{
		root = Node_internal(NODETYPE_NONE, nullptr);
		return;
	}
	else if (first_c == '{') // If file is map
	{
		root = Node_internal(NODETYPE_MAP, nullptr); // Create map at tree root
		nodestack_.push(NODEWAY_MAP); // Map into stack
		nodestack_.push(NODEWAY_COMMA_MAP); // Push comma such if it was comma first in the map
		nodestack_.push(NODEWAY_KEY); // Key into stack (to read key before value)
	}
	else if (first_c == '[') // If file is list
	{
		root = Node_internal(NODETYPE_LIST, nullptr); // Create list at tree root
		nodestack_.push(NODEWAY_LIST); // List into stack
		nodestack_.push(NODEWAY_COMMA_LIST); // Push comma such if it was comma first in the list
	}
	else throw LSCL::Exception::Exception_nodebuilder("Unable to detect root type (list or map). All file contents should be inside [] or {} brackets.", ss_.get_line());
	
	ss_.eat_next_char();
	ss_.skip_spaces();
	
	Node_internal *workpoint = &root; // Pointer to current workpoint (where structure is parsed)
	
	while (ss_ && !nodestack_.empty() && workpoint)
	{
		const char c = ss_.peek_next_char(); // Get one char from stream
		const NODEWAY last = nodestack_.top(); // Get type of last 
#ifdef __DEBUG
	std::cout << "Parsing type " << last << std::endl;
#endif
		switch (c)
		{
			case '{':
			{
				nodestack_.push(NODEWAY_MAP); // Map into stack
				INSERT_CONTAINER(NODETYPE_MAP, "map");
				nodestack_.push(NODEWAY_COMMA_MAP); // Push comma such if it was comma first in the map
				nodestack_.push(NODEWAY_KEY); // Key into stack (to read key before value)
				
				break;
			}
			case '}':
			{
				if (last == NODEWAY_COMMA_MAP) nodestack_.pop(); // Map end
				if (last == NODEWAY_MAP) nodestack_.pop();
				else throw LSCL::Exception::Exception_nodebuilder("\'}\' bracket appeared in wrong place", ss_.get_line());
				
				workpoint = workpoint->parent;
				
				break;
			}
			case '[':
			{
				nodestack_.push(NODEWAY_LIST); // List into stack
				INSERT_CONTAINER(NODETYPE_LIST, "list");
				nodestack_.push(NODEWAY_COMMA_LIST); // Push comma such if it was comma first in the list
				
				break;
			}
			case ']':
			{
				if (last == NODEWAY_COMMA_LIST) nodestack_.pop(); // List end
				if (last == NODEWAY_LIST) nodestack_.pop();
				else throw LSCL::Exception::Exception_nodebuilder("\']\' bracket appeared in wrong place", ss_.get_line());
				
				workpoint = workpoint->parent;
				
				break;
			}
			case ',':
			{
				     if (last == NODEWAY_LIST) nodestack_.push(NODEWAY_COMMA_LIST);
				else if (last == NODEWAY_MAP)  nodestack_.push(NODEWAY_COMMA_MAP);
				else if (last != NODEWAY_COMMA_LIST && last != NODEWAY_COMMA_MAP)
					throw LSCL::Exception::Exception_nodebuilder("Comma appeared in wrong place", ss_.get_line());
				
				break;
			}
			case ':':
			{
				if (last != NODEWAY_COMMA_MAP) throw LSCL::Exception::Exception_nodebuilder("Key:value delimiter \':\' in the wrong place", ss_.get_line());
				break;
			}
			case '#':
			{
				// process_directive()
				break;
			}
			default:
			{
				std::string current_scalar = process_scalar(); // Get scalar string
#ifdef __DEBUG
				std::cout << "Got scalar: |" << current_scalar << '|' << std::endl;
#endif
				
				switch (last)
				{
					case NODEWAY_KEY:
					{
						key = current_scalar;
						nodestack_.pop(); // Pop key nodeway flag
						break;
					}
					case NODEWAY_COMMA_MAP:
					{
						workpoint->values_map.insert({
							key,
							Node_internal(workpoint, current_scalar)
						});
						nodestack_.pop(); // Pop comma_map nodeway flag
						break;
					}
					case NODEWAY_COMMA_LIST:
					{
						workpoint->values_list.push_back(Node_internal(workpoint, current_scalar));
#ifdef __DEBUG
std::cout << "Pushed new scalar into list, size=" << workpoint->values_list.size() << " type=" << workpoint->values_list[0].type << std::endl;
#endif
						nodestack_.pop(); // Pop comma_list nodeway flag
						break;
					}
					case NODEWAY_MAP:
					case NODEWAY_LIST:
					{
						throw LSCL::Exception::Exception_nodebuilder("Nodes in map or list are not comma-separated", ss_.get_line());
						break;
					}
					
					default: { throw LSCL::Exception::Exception_nodebuilder("Trying to insert scalar into wrong container", ss_.get_line()); break; }
				}
			}
		}
		
		ss_.eat_next_char(); // Go to next char
		ss_.skip_spaces(); // Skip blank space before next trigger
		
		//else { throw LSCL::Exception::Exception_nodebuilder("Incorrect syntax", ss_.get_line()); }
		
	}
#ifdef __DEBUG
	std::cout << "Nodebuilder constructor end" << std::endl;
#endif
}





// Processing scalar value, quoted or not
std::string Builder::process_scalar(void)
{
#ifdef __DEBUG
	std::cout << "Process scalar start" << std::endl;
#endif
	std::string answer;
	register char c;
	c = ss_.peek_next_char();
	
	bool escaped        = false;
	bool quote_single   = false;
	bool quote_double   = false;
	bool skiping_spaces = true;
	bool triangle       = false; // Triangle brackets
	
	while (c)
	{
		c = ss_.pop_next_char();
		
		// Skipping blank space before value start and in the beginning
		// of each line (if it is multiline scalar value)
		if (skiping_spaces)
		{
			while (c == ' ' || c == '\t') c = ss_.pop_next_char();
		}
		skiping_spaces = false;
		
		// If function meets double quote
		if (c == '\"' && !escaped && !quote_single)
		{
			if (quote_double) break; // scalar end
			quote_double = true; // Change quotation status
			continue;
		}
		
		// If function meets single quote
		if (c == '\'' && !escaped && !quote_double)
		{
			if (quote_single) break; // scalar end
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
					c = ss_.peek_next_char();
					// Pushing hex symbol code into string
					while (
						('0' <= c && c <= '9') ||
						('a' <= c && c <= 'f') ||
						('A' <= c && c <= 'F')
					)
					{
						current_hex_value.push_back(ss_.pop_next_char());
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
					else throw LSCL::Exception::Exception_nodebuilder("Symbol hex code in string is defined incorrectly. Correct format is: \\x< hex code >;", ss_.get_line());
					
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
		
		
		if (!escaped)
		{
			
			// If function meets line-break
			if (c == '\n')
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
			
			if (
				c == '[' || c == ']' ||
				c == '{' || c == '}' ||
				c == ':' || c == '#' || c == ','
			)
			{
				if (!quote_single && !quote_double && !triangle) break; // Special symbol stops scalar parsing
				else throw LSCL::Exception::Exception_nodebuilder("Scalar contains forbidden symbol: \'" + std::to_string(c) + "\'", ss_.get_line());
			}
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
	
#ifdef __DEBUG
	std::cout << "Process scalar end" << std::endl;
#endif
	
	return answer;
}

} // Namespace Nodebuilder

} // Namespace LSCL
