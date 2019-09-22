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


inline bool is_spacer(const char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
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
		std::string current_scalar = process_scalar(); // Get scalar string
		c = ss_.peek_next_char();
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
				process_directive(workpoint);
				break;
			}
			case '&': // Creating link to object
			{
				ss_.eat_next_char(); // Eat ampersand
				ss_.skip_spaces();
				c = ss_.peek_next_char();
				if (c == '<' || c == '\'' || c == '\"')
				{
					link_name = process_scalar(); // Get link name as scalar
				}
				else
				{
					link_name = process_single_word(); // Get link name as a single word
					// After word we always should have delimiter (space or line-break)
					c = ss_.peek_next_char();
					if (c != ' ' && c != '\n' && c != '\t') throw LSCL::Exception::Exception_nodebuilder("Single word \"" + link_name + "\" is ending with \'" + std::to_string(c) + "\'", filename_, ss_.get_line());
				}
				
				character_preserved = is_scalar_terminating_symbol(ss_.peek_next_char()); // If we need to preserve next character in stream
				link_creation = true;
#ifdef __DEBUG
				std::cout << "Got link name: |" << link_name << '|' << std::endl;
#endif
				break;
			}
			case '*': // Referencing by link
			{
				ss_.eat_next_char(); // Eat asterisk
				std::string linked_node_name;
				
				if (c == '<' || c == '\'' || c == '\"') linked_node_name = process_scalar(); // Get link name as scalar
				else linked_node_name = process_single_word(); // Get link name as a single word
				character_preserved = is_scalar_terminating_symbol(ss_.peek_next_char()); ; // If we need to preserve next character in stream
				
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
				std::string current_scalar = process_scalar(); // Get scalar string
#ifdef __DEBUG
				std::cout << "Got scalar: |" << current_scalar << '|' << std::endl;
#endif
				// If we need to preserve next character in stream
				character_preserved = is_scalar_terminating_symbol(ss_.peek_next_char()); ;
				
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
		
		if (!character_preserved) ss_.eat_next_char(); // Go to next char
		else character_preserved = false;
		ss_.skip_spaces(); // Skip blank space before next trigger
		
	} // End while
	
#ifdef __DEBUG
	std::cout << "Building node end" << std::endl;
#endif
}





// Processing scalar value, quoted or not
std::string Builder::process_scalar(void)
{
#ifdef __DEBUG
	std::cout << "Process scalar start" << std::endl;
#endif
	std::string answer;
	
	bool escaped        = false;
	bool quote_single   = false;
	bool quote_double   = false;
	bool skiping_spaces = true;
	bool triangle       = false; // Triangle brackets
	
	ss_.skip_spaces();
	register char c = ss_.peek_next_char();
	// If function meets triangle bracket in the beginning
	if (c == '<')
	{
		triangle = true; // Flag that we a re preserving \n charactes inside strings
		ss_.eat_next_char();
	}
	
	while (c)
	{
		c = ss_.peek_next_char();
		
		// Skipping blank space before value start and in the beginning
		// of each line (if it is multiline scalar value)
		if (skiping_spaces)
		{
			while (c == ' ' || c == '\t') c = ss_.pop_next_char();
		}
		skiping_spaces = false;
		
		if (!escaped)
		{
			
			// If function meets line-break
			if (c == '\n')
			{
				// If line-break is inside quotes
				// (line-break outside quotes is a delimiter in lists and maps)
				// \n
				if (triangle) answer.push_back('\n');
				skiping_spaces = true;
				ss_.eat_next_char();
				continue;
			}
			
			// If we reached scalar end
			// If (character is not inside quoted string) and (character is > or terminating)
			if (!quote_single && !quote_double && (c == '>' || is_scalar_terminating_symbol(c)))
			{
				if (!triangle || c == '>') break; // Special symbol stops scalar parsing
				else throw LSCL::Exception::Exception_nodebuilder("Scalar contains forbidden symbol: \'" + std::to_string(c) + "\' \'" + std::string(1, c) + "\'", filename_, ss_.get_line());
			}
		}
		
		ss_.eat_next_char(); // It is not scalar-terminating symbol, we can destroy symbol in stream
		
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
							answer.push_back(e);
						}
					}
					// Else throw exception
					else throw LSCL::Exception::Exception_nodebuilder("Symbol hex code in string is defined incorrectly (" + current_hex_value + "). Correct format is: \\x< hex code >;", filename_, ss_.get_line());
					
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
		
		// If function meets escaping character
		// If it's not escaped, then start escape
		if (c == '\\' && !escaped && !quote_single) { escaped = true; continue; }
		// Else we have to end escape (tell, that it's normal symbol, need it to work with "\" symbol)
		// And other symbols, which shouldn`t be escaped
		else escaped = false;
		
		// ==== End of escaped characters processing, under it 'ESCAPED' always FALSE
		
		// Push current character into collector
		answer.push_back(c);
	} // End while
	
	// Erase white spaces in the end of unquoted string
	if (!quote_double && !quote_single)
		while (answer.back() == ' ' || answer.back() == '\t' || answer.back() == '\n') answer.pop_back();
	
	// Process closing triangle bracket
	else
		if (triangle)
		{
			ss_.skip_spaces();
			if (ss_.peek_next_char() == '>') ss_.eat_next_char();
			else throw LSCL::Exception::Exception_nodebuilder("Opening triangle bracket \"<\" without closing one \">\"", filename_, ss_.get_line());
		}
	
#ifdef __DEBUG
	std::cout << "Process scalar end" << std::endl;
#endif
	
	return answer;
}



// Processing plain text word without escaped characters quotes, spaces and line-breaks
std::string Builder::process_single_word(void)
{
#ifdef __DEBUG
	std::cout << "Process single word start" << std::endl;
#endif
	std::string answer;
	
	ss_.skip_spaces();
	register char c;
	
	do
	{
		c = ss_.peek_next_char();
		
		// Stop reading word if we meet terminating symbol
		if ( 
			c == ' '  || c == '\n' || c == '\t' || c == '\"' || c == '\'' || 
			c == '<'  || c == '>'  || c == '\\' || is_scalar_terminating_symbol(c)
		)
		{
			break;
		}
		
		ss_.eat_next_char(); // It is not scalar-terminating symbol, we can destroy symbol in stream
		
		// Push current character into collector
		answer.push_back(c);
	}
	while (c); // End while
	
#ifdef __DEBUG
	std::cout << "Process single word end" << std::endl;
#endif
	
	return answer;
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


void Builder::process_directive(Node_internal *workpoint) // Processing of directive
{
	
}


} // Namespace Nodebuilder

} // Namespace LSCL
