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


// Adding link into links map
#define LINK_CREATION(x)         \
	if (link_creation)           \
	{                            \
		links_.insert({          \
			link_name,           \
			(x)                  \
		});                      \
		link_creation = false;   \
	}                            \


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
			throw LSCL::Exception::Exception_nodebuilder("Adding " + std::string(t_name) + " into map", filename_, ss_.get_line()); \
		workpoint = &(inserted->second);                 \
	}                                                    \
	else if (last == NODEWAY_COMMA_LIST)                 \
	{                                                    \
		workpoint->values_list.push_back(                \
			Node_internal((t), workpoint)                \
		);                                               \
		workpoint = &( workpoint->values_list.back() );  \
	}                                                    \
	else throw LSCL::Exception::Exception_nodebuilder("Adding " + std::string(t_name) + " into non-container node", filename_, ss_.get_line()); \
	                           \
	LINK_CREATION(workpoint)   \
	

namespace LSCL
{

namespace Nodebuilder
{



// If symbol is one of which terminate scalar parsing
inline bool is_scalar_terminating_symbol(const char c)
{
	return (
		c == '\0' || c == EOF ||
		c == '['  || c == ']' ||
		c == '{'  || c == '}' ||
		c == ':'  || c == '#' ||
		c == ','
	);
}

inline void print_s_pair(std::pair<std::string, bool> s_pair)
{
	std::cout << "Scalar |" << s_pair.first << "|, preserve_char = " << s_pair.second << std::endl;
}


std::string get_nodeway_name(LSCL::Nodebuilder::Builder::NODEWAY type)
{
	switch (type)
	{
		case Builder::NODEWAY_LIST:       { return "LIST";       break; }
		case Builder::NODEWAY_MAP:        { return "MAP";        break; }
		case Builder::NODEWAY_SCALAR:     { return "SCALAR";     break; }
		case Builder::NODEWAY_COMMA_LIST: { return "COMMA_LIST"; break; }
		case Builder::NODEWAY_COMMA_MAP:  { return "COMMA_MAP";  break; }
		case Builder::NODEWAY_KEY:        { return "KEY";        break; }
		default: return std::to_string(type);
	}
}

/**
 * Class constructor.
 * Initializes class from input stream,
 * reads first portion of characters into rx buffer
 * 
 * Warning! This is the most critical section, write code as effective as possible.
 * 
 */
Builder::Builder(std::istream &input, const std::string &filename) :
	ss_(input),
	filename_(filename)
{
	build_tree();
	assign_links();
}


void Builder::build_tree(void)
{
#ifdef __DEBUG
	std::cout << "\nBuilding node start" << std::endl;
#endif
	
	std::string key; // Key for map
	std::string link_name; // Name of link to be referenced
	
	ss_.skip_spaces(); // Go to first non-space symbol
	
	// Detecting root type (list or map)
	const char first_c = ss_.peek_next_char(); // Get first char from stream
	register char c;
	bool character_preserved = false; // Flag for cases when functional symbol is eaten by algorithm
	bool link_creation = false; // If we have to link the following node
	
	if (first_c == EOF || first_c == '\0') // If no structure in file
	{
		root = std::make_shared<Node_internal>(NODETYPE_NONE, nullptr);
		return;
	}
	else if (first_c == '{') // If file is map
	{
		root = std::make_shared<Node_internal>(NODETYPE_MAP, nullptr); // Create map at tree root
		nodestack_.push(NODEWAY_MAP); // Map into stack
		nodestack_.push(NODEWAY_COMMA_MAP); // Push comma such if it was comma first in the map
		nodestack_.push(NODEWAY_KEY); // Key into stack (to read key before value)
	}
	else if (first_c == '[') // If file is list
	{
		root = std::make_shared<Node_internal>(NODETYPE_LIST, nullptr); // Create list at tree root
		nodestack_.push(NODEWAY_LIST); // List into stack
		nodestack_.push(NODEWAY_COMMA_LIST); // Push comma such if it was comma first in the list
	}
	else
	{
		// Just one scalar in file
		auto s_pair = process_scalar(); // Get scalar string
		print_s_pair(s_pair);
		std::string current_scalar = s_pair.first;
		ss_.eat_next_char();
		c = ss_.skip_spaces();
		std::cout << '|' << c << '|' << std::endl;
		if (c != EOF && c != '\0') throw LSCL::Exception::Exception_nodebuilder("Incorrect scalar format", filename_, ss_.get_line());
		root = std::make_shared<Node_internal>(nullptr, current_scalar); // Create scalar at tree root
		return;
	}
	
	ss_.eat_next_char();
	ss_.skip_spaces();
	
	Node_internal *workpoint = root.get(); // Pointer to current workpoint (where structure is parsed)
	
	while (ss_ && !nodestack_.empty() && workpoint)
	{
		c = ss_.peek_next_char(); // Get one char from stream
		const NODEWAY last = nodestack_.top(); // Get type of last 
#ifdef __DEBUG
	std::cout << "Parsing type " << get_nodeway_name(last) << std::endl;
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
				if (last             == NODEWAY_KEY)       nodestack_.pop(); // Map end
				if (nodestack_.top() == NODEWAY_COMMA_MAP) nodestack_.pop();
				if (nodestack_.top() == NODEWAY_MAP)       nodestack_.pop();
				else throw LSCL::Exception::Exception_nodebuilder("\'}\' bracket appeared in wrong place", filename_, ss_.get_line());
				
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
				if (last             == NODEWAY_COMMA_LIST) nodestack_.pop(); // List end
				if (nodestack_.top() == NODEWAY_LIST)       nodestack_.pop();
				else throw LSCL::Exception::Exception_nodebuilder("\']\' bracket appeared in wrong place", filename_, ss_.get_line());
				
				workpoint = workpoint->parent;
				
				break;
			}
			case ',':
			{
				     if (last == NODEWAY_LIST)   nodestack_.push(NODEWAY_COMMA_LIST);
				else if (last == NODEWAY_COMMA_MAP) { nodestack_.push(NODEWAY_KEY); }
				else if (last == NODEWAY_MAP)  { nodestack_.push(NODEWAY_COMMA_MAP); nodestack_.push(NODEWAY_KEY); }
				else if (last != NODEWAY_COMMA_LIST && last != NODEWAY_COMMA_MAP)
					throw LSCL::Exception::Exception_nodebuilder("Comma appeared in wrong place", filename_, ss_.get_line());
				
#ifdef __DEBUG
				std::cout << "Comma" << std::endl;
#endif
				
				break;
			}
			case ':':
			{
				if (last == NODEWAY_KEY) throw LSCL::Exception::Exception_nodebuilder("No key before key:value delimiter \':\'", filename_, ss_.get_line());
				if (last != NODEWAY_COMMA_MAP) throw LSCL::Exception::Exception_nodebuilder("Key:value delimiter \':\' in the wrong place", filename_, ss_.get_line());
				break;
			}
			case '#':
			{
				// process_directive(workpoint);
				break;
			}
			case '&': // Creating link to object
			{
				ss_.eat_next_char(); // Eat ampersand
				ss_.skip_spaces();
				c = ss_.peek_next_char();
				if (c == '<' || c == '\'' || c == '\"')
				{
					auto s_pair = process_scalar(); // Get link name as scalar
					link_name = s_pair.first;
					character_preserved = s_pair.second;
				}
				else
				{
					auto s_pair = process_single_word(); // Get link name as a single word
					link_name = s_pair.first;
					character_preserved = s_pair.second;
					// After word we always should have delimiter (space or line-break)
					c = ss_.peek_next_char();
					if (c != ' ' && c != '\n' && c != '\t') throw LSCL::Exception::Exception_nodebuilder("Link creation (single word) \"" + link_name + "\" is ending with \'" + std::to_string(c) + "\'", filename_, ss_.get_line());
				}
				
				//character_preserved = is_scalar_terminating_symbol(ss_.peek_next_char()); // If we need to preserve next character in stream
				link_creation = true;
#ifdef __DEBUG
				std::cout << "Got link name: |" << link_name << '|' << std::endl;
#endif
				break;
			}
			case '*': // Referencing by link
			{
				ss_.eat_next_char(); // Eat asterisk
				c = ss_.skip_spaces();
				std::string linked_node_name;
				if (c == '<' || c == '\'' || c == '\"')
				{
					auto s_pair = process_scalar(); // Get link name as scalar
					linked_node_name = s_pair.first;
					character_preserved = s_pair.second;
				}
				else 
				{
					auto s_pair = process_single_word(); // Get link name as a single word
					linked_node_name = s_pair.first;
					character_preserved = s_pair.second;
				}
				//character_preserved = is_scalar_terminating_symbol(ss_.peek_next_char()); ; // If we need to preserve next character in stream
				
				// Save link with its name
				Node_internal *node_to_work_with;
				// If link is value in key-value pair
				if (last == NODEWAY_COMMA_MAP)
				{
					// Create link node
					workpoint->values_map.insert({
						key,
						Node_internal(NODETYPE_LINK, workpoint)
					});
					node_to_work_with = &workpoint->values_map[key];
				}
				// If link is element of list
				else if (last == NODEWAY_COMMA_LIST)
				{
					// Create link node
					workpoint->values_list.push_back(
						Node_internal(NODETYPE_LINK, workpoint)
					);
					node_to_work_with = &workpoint->values_list.back();
				}
				
				// Save link to this scalar into links map, if needed
				LINK_CREATION(node_to_work_with);
				
				nodestack_.pop(); // Pop comma_list or comma_map nodeway flag
				
				linked_nodes_.insert({
					linked_node_name,
					node_to_work_with
				});
				
				break;
			}
			default:
			{
				auto s_pair = process_scalar(); // Get scalar string
				std::string current_scalar = s_pair.first;
				character_preserved = s_pair.second;
				// If we need to preserve next character in stream
				// character_preserved = is_scalar_terminating_symbol(ss_.peek_next_char());
				
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
						// Insert new scalar into map
						workpoint->values_map.insert({
							key,
							Node_internal(workpoint, current_scalar)
						});
						// Save link to this scalar into links map
						LINK_CREATION(&workpoint->values_map[key]);
						
						nodestack_.pop(); // Pop comma_map nodeway flag
						break;
					}
					case NODEWAY_COMMA_LIST:
					{
						// Insert new scalar into list
						workpoint->values_list.push_back(Node_internal(workpoint, current_scalar));
						nodestack_.pop(); // Pop comma_list nodeway flag
						// Save link to this scalar into links map
						LINK_CREATION(&workpoint->values_list.back());
						
						break;
					}
					case NODEWAY_MAP: // FALLTHGOUGH
					case NODEWAY_LIST:
					{
						throw LSCL::Exception::Exception_nodebuilder("Nodes in map or list are not comma-separated", filename_, ss_.get_line());
						break;
					}
					
					default: { throw LSCL::Exception::Exception_nodebuilder("Trying to insert scalar into wrong container", filename_, ss_.get_line()); break; }
				}
				
			} // End default
		} // End case
		
		const size_t current_line = ss_.get_line();
		if (!character_preserved) ss_.eat_next_char(); // Go to next char
		else character_preserved = false;
		ss_.skip_spaces(); // Skip blank space before next trigger
		if (current_line != ss_.get_line()) // If newline is delimiter
		{
			switch (nodestack_.top())
			{
				case NODEWAY_LIST: { nodestack_.push(NODEWAY_COMMA_LIST); break; }
				case NODEWAY_MAP:  { nodestack_.push(NODEWAY_COMMA_MAP);  break; }
				default: break;
			}
		}
		
	} // End while
	
#ifdef __DEBUG
	std::cout << "Building node end" << std::endl;
#endif
}





// Processing scalar value, quoted or not
std::pair<std::string, bool> Builder::process_scalar(void)
{
#ifdef __DEBUG
	std::cout << "Process scalar start (c = " << std::to_string(ss_.peek_next_char()) << ")" << std::endl;
#endif
	std::string accum; // Resulting string
	bool preserve_char = false; // If we need to preserve current char in stream for the next iteration
	
	register char c; // Current char
	bool escaped        = false; // If current character is escaped
	bool quote          = false; // If scalar is in any quotes
	bool quote_single   = false; // If scalar is in ' '
	bool quote_double   = false; // If scalar is in " "
	bool skiping_spaces = true;  // If we need to skip spaces to the next non-spacer char
	bool triangle       = false; // Triangle brackets
	
	c = ss_.skip_spaces();
	
	// If function meets triangle bracket in the beginning
	if (c == '<')
	{
		triangle = true; // Flag that we are preserving \n charactes inside strings
		ss_.eat_next_char(); // Eat '<'
		c = ss_.skip_spaces(); // Get next
	}
	
	// If function meets single quote
	if (c == '\'')
	{
		quote = true;
		quote_single = true;
		ss_.eat_next_char(); // Eat '
	}
	// If function meets double quote
	else if (c == '\"')
	{
		quote = true;
		quote_double = true;
		ss_.eat_next_char(); // Eat "
	}
	
	while (c)
	{
		c = ss_.peek_next_char(); // Get next
		
		// Skipping blank space before value start and in the beginning
		// of each line (if it is multiline scalar value)
		if (skiping_spaces)
		{
			c = ss_.skip_spaces();
			skiping_spaces = false;
		}
		
		if (!escaped) // If current character is not escaped
		{
			// If function meets line-break
			if (c == '\n' && !triangle)
			{
				// If line-break is inside quotes
				// (line-break outside quotes is a delimiter in lists and maps)
				if (!quote) break;
				skiping_spaces = true;
				ss_.eat_next_char(); // Eat \n
				continue;
			}
			
			// If we reached scalar end
			// If (character is not inside quoted string) and (character is > or terminating)
			if (!quote)
			{
				if (is_scalar_terminating_symbol(c) || (triangle && c == '>'))
				{
					if (triangle && c != '>') throw LSCL::Exception::Exception_nodebuilder("Forbidden symbol \"" + std::to_string(c) + "\" inside triangle brackets", filename_, ss_.get_line());
					preserve_char = true;
					break;
				}
			}
			
			// If function meets double quote
			if (!quote_single && c == '\"')
			{
				preserve_char = !quote_double;
				break; // scalar end
			}
			
			// If function meets single quote
			if (!quote_double && c == '\'')
			{
				preserve_char = !quote_single;
				break; // scalar end
			}
		}
		
		ss_.eat_next_char(); // It is not scalar-terminating symbol, we can destroy symbol in stream
		
		// Escaped characters
		if (escaped)
		{
			switch (c)
			{
				case 'n': { accum.push_back('\n'); break; } // Line-break character
				case 't': { accum.push_back('\t'); break; } // Tabulator character
				case 's': { accum.push_back(' ');  break; } // Space
				case 'r': { accum.push_back('\r'); break; } // Carriage return
				case 'x':
				{
					// Hex representation
					std::string current_hex_value;
					do
					{
						c = ss_.pop_next_char();
						current_hex_value.push_back(c);
					}
					while (
						('0' <= c && c <= '9') ||
						('a' <= c && c <= 'f') ||
						('A' <= c && c <= 'F')
					);
					
					if (c == ';') // If ending symbol is valid (; in the end if hex symbol surrogate)
					{
						// Convert hex string into symbol (maybe )
						
						current_hex_value.pop_back();
						std::vector<uint32_t> surrogate_vector = { (uint32_t)strtoul(current_hex_value.c_str(), NULL, 16) };
						current_hex_value = uint32_vector_to_string(surrogate_vector);
						for (char e : current_hex_value)
						{
							accum.push_back(e);
						}
					}
					// Else throw exception
					else throw LSCL::Exception::Exception_nodebuilder("Symbol hex code in string is defined incorrectly (" + current_hex_value + "). Correct format is: \\x< hex code >;", filename_, ss_.get_line());
					
					break;
				}
				// In all other cases just save escaped character
				default:
				{
					accum.push_back(c);
					break;
				}
			} // End switch
			
			escaped = false; // End escape
			continue;
		} // endif (escaped)
		
		// If function meets escaping character
		// If it's not escaped, then start escape
		if (c == '\\' && !escaped && !quote_single) { escaped = true; continue; }
		// Else we have to end escape (tell, that it's normal symbol, need it to work with "\" symbol)
		// And other symbols, which shouldn`t be escaped
		else escaped = false;
		
		// ==== End of escaped characters processing, under it 'ESCAPED' always FALSE
		
		// Push current character into collector
		accum.push_back(c);
		
	} // End while
	
	// Erase white spaces in the end of unquoted string
	if (!quote) while (is_spacer(accum.back())) accum.pop_back();
	
	// Process closing triangle bracket
	if (triangle)
	{
		if (!preserve_char) ss_.eat_next_char();
		preserve_char = false;
		ss_.skip_spaces();
		if (ss_.peek_next_char() == '>') ss_.eat_next_char();
		else throw LSCL::Exception::Exception_nodebuilder("Opening triangle bracket \"<\" without closing one \">\"", filename_, ss_.get_line());
	}
	
#ifdef __DEBUG
	std::cout << "Process scalar end: |" << accum << '|' << std::endl;
#endif
	return std::make_pair(accum, preserve_char);
}


// Processing plain text word without escaped characters quotes, spaces and line-breaks
std::pair<std::string, bool> Builder::process_single_word(void)
{
#ifdef __DEBUG
	std::cout << "Process single word start (c = " << std::to_string(ss_.peek_next_char()) << ")" << std::endl;
#endif
	std::string accum;
	bool preserve_char = false; // If we need to preserve current char in stream for the next iteration
	
	ss_.skip_spaces();
	register char c;
	
	do
	{
		c = ss_.peek_next_char();
		
		// Stop reading word if we meet terminating symbol
		if (is_spacer(c) || c == '\"' || c == '\'' || c == '<' || c == '>'  || c == '\\')
		{
			break;
		}
		if (is_scalar_terminating_symbol(c))
		{
			preserve_char = true;
			break;
		}
		
		ss_.eat_next_char(); // It is not scalar-terminating symbol, we can destroy symbol in stream
		
		// Push current character into collector
		accum.push_back(c);
	}
	while (c); // End while
	
#ifdef __DEBUG
	std::cout << "Process single word end: |" << accum << '|' << std::endl;
#endif
	
	return std::make_pair(accum, preserve_char);
}



void Builder::assign_links(void)
{
	for (auto &e : linked_nodes_)
	{
		auto existing_link = links_.find(e.first);
		// If such link name was defined
		if (existing_link != links_.end())
		{
			if (e.second->type == NODETYPE_LINK)
			{
				e.second->linked = existing_link->second;
			}
			else throw LSCL::Exception::Exception_nodebuilder("Using undefined link \"" + e.first + "\"", filename_, ss_.get_line());
		}
	}
}

/*
void Builder::process_directive(Node_internal *workpoint) // Processing of directive
{
	
}
*/

} // Namespace Nodebuilder

} // Namespace LSCL
