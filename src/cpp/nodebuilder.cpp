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
#include <fstream>

#include "nodebuilder.hpp"
#include "exception.hpp"
#include "global.hpp"

namespace LSCL
{

namespace Nodebuilder
{



/**
 * Class constructor.
 * Initializes class from input stream or file
 * 
 * Warning! This is the most critical section, write code as effective as possible.
 * 
 */

Builder::Builder(const std::string &filename) :
	filename_(filename)
{
	if (filename.empty()) throw LSCL::Exception::Exception_nodebuilder("Empty file name is given", filename, get_line());
	std::ifstream in_file(filename.c_str());
	if(!in_file.good()) throw LSCL::Exception::Exception_nodebuilder("Error opening file", filename, get_line());
	build_tree(in_file);
	return;
}

Builder::Builder(std::istream &input)
{
	if(!input.good() || input.eof()) return;
	else build_tree(input);
}


/**
 * Class destructor
 * 
 */
Builder::~Builder()
{
   delete(scanner_);
   scanner_ = nullptr;
   delete(parser_);
   parser_ = nullptr;
}


/**
 * This function uses flex-bison parser to build tree
 * 
 */
void Builder::build_tree(std::istream &input)
{
	delete(scanner_);
	try
	{
		scanner_ = new LSCL_Scanner(&input);
	}
	catch (std::bad_alloc &ba)
	{
		throw LSCL::Exception::Exception_nodebuilder("Error to allocate memory for scanner: (" + std::string(ba.what()) + ")");
	}
	
	delete(parser_); 
	try
	{
		parser_ = new LSCL_Parser(
			*scanner_, // scanner 
			*this     // builder
		);
	}
	catch (std::bad_alloc &ba)
	{
		throw LSCL::Exception::Exception_nodebuilder("Error to allocate memory for parser: (" + std::string(ba.what()) + ")");
	}
	
	if (parser_->parse() != 0)
	{
		throw LSCL::Exception::Exception_nodebuilder("Parser failed");
	}
}




/*
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
*/


/*
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
*/


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
			else throw LSCL::Exception::Exception_nodebuilder("Using undefined link \"" + e.first + "\"", filename_, get_line());
		}
	}
}


} // Namespace Nodebuilder

} // Namespace LSCL
