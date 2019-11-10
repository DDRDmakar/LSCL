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
#include "script.hpp"

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
	std::cout << "\033[1;36mBuilder constructor\033[0m\n";
	if(!input.good() || input.eof()) return;
	else build_tree(input);
}


/**
 * Class destructor
 * 
 */
Builder::~Builder(void)
{
   delete(scanner_);
   scanner_ = nullptr;
   delete(parser_);
   parser_ = nullptr;
}


void Builder::setroot(Node_internal *newroot)
{
	root_ = std::unique_ptr<Node_internal>(newroot);
}


const std::string& Builder::get_filename(void) const
{
	return filename_;
}

Node Builder::getroot(void) const
{
	return Node(root_);
}

Node_internal* Builder::getroot_internal(void) const
{
	return root_.get();
}

Node_internal* Builder::release_root(void)
{
	return root_.release();
}

/**
 * This function uses flex-bison parser to build tree
 * 
 */
void Builder::build_tree(std::istream &input)
{
	// ================== INITIAL PART
	
	
	
	// ================== FLEX-BISON PART
	
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
			*this      // builder
		);
	}
	catch (std::bad_alloc &ba)
	{
		throw LSCL::Exception::Exception_nodebuilder("Error to allocate memory for parser: (" + std::string(ba.what()) + ")");
	}
	
	int parse_status = parser_->parse();
	if (parse_status != 0)
	{
		throw LSCL::Exception::Exception_nodebuilder("Parser failed (code " + std::to_string(parse_status) + ")");
	}
	
	// ================== POST-PROCESSING PART
	
	assign_includes();
	
	assign_links();
}


size_t Builder::get_line(void) const
{
	return scanner_->lineno();
}




std::string Builder::process_scalar_plaintext(const std::string &input) const
{
	std::string accum;
	std::string current_hex_value;
	
	bool escaped        = false;
	bool skiping_spaces = true;
	bool readhex        = false;
	
	for (const register char c : input)
	{
		// Skipping blank space before value start and in the beginning
		// of each line (if it is multiline scalar value)
		if (skiping_spaces)
		{
			if (c == ' ' || c == '\t') continue;
			else skiping_spaces = false;
		}
		
		// Escaped characters
		if (escaped)
		{
			switch (c)
			{
				case 'n': { accum.push_back('\n'); break; } // Line-break character
				case 't': { accum.push_back('\t'); break; } // Tabulator character
				case 's': { accum.push_back(' ');  break; } // Space
				case 'r': { accum.push_back('\r'); break; } // Carriage return
				case 'x': {                                 // Hex representation
					readhex = true;
					current_hex_value.clear();
					break;
				}
				// In all other cases just save escaped character
				default:
				{
					accum.push_back(c);
					break;
				}
			}
			escaped = false; // End escape
			continue;
		} // endif escaped
		
		else // If not escaped
		{
			// Begin and end of scalar (quotes)
			if (c == '\"') continue;
			
			// If function meets line-break
			if (c == '\n')
			{
				// If line-break is inside quotes
				skiping_spaces = true;
				continue;
			}
		}
		
		if (readhex)
		{
			if (
				('0' <= c && c <= '9') ||
				('a' <= c && c <= 'f') ||
				('A' <= c && c <= 'F')
			) current_hex_value.push_back(c);
			else if (c == ';') // If ending symbol is valid (; in the end if hex symbol surrogate)
			{
				// Convert hex string into symbol
				std::vector<uint32_t> surrogate_vector = { (uint32_t)strtoul(current_hex_value.c_str(), NULL, 16) };
				current_hex_value = uint32_vector_to_string(surrogate_vector);
				for (char e : current_hex_value)
				{
					accum.push_back(e);
				}
				readhex = false;
			}
			// Else throw exception
			else
			{
				throw LSCL::Exception::Exception_nodebuilder("Symbol hex code in string is defined incorrectly (" + current_hex_value + "). Correct format is: \\x< hex code >;", filename_, get_line());
			}
			
			continue;
		}
		
		// If function meets escaping character
		// If it's not escaped, then start escape
		if (c == '\\') { escaped = !escaped; continue; }
		
		// Push current character into collector
		accum.push_back(c);
	} // End for
	
	// Erase white spaces in the end of unquoted string
	while (is_spacer(accum.back())) accum.pop_back();
	
	return accum;
}




std::string Builder::process_scalar_quotes_single(const std::string &input, const int preserve_newline) const
{
	std::string accum;
	
	bool quote          = false;
	bool skiping_spaces = false;
	bool begin          = true;
	
	if (input.front() != '\'' || input.back() != '\'') throw Exception::Exception_nodebuilder("Input scalar form is incorrect");
	
	for (const register char c : input)
	{
		// Skipping blank space before value start and in the beginning
		// of each line (if it is multiline scalar value)
		if (skiping_spaces && preserve_newline != 2)
		{
			if (c == ' ' || c == '\t') continue;
			else skiping_spaces = false;
		}
		
		// Do not trigger on first quote
		if (begin)
		{
			begin = false;
			continue;
		}
		
		// One single quote escapes another one ('' in config will be parsed as ')
		if (c == '\'' && !begin)
		{
			if (quote) accum.push_back('\'');
			quote = !quote;
			continue;
		}
		quote = false;
		
		// If function meets line-break
		if (c == '\n')
		{
			if (preserve_newline) accum.push_back('\n');
			skiping_spaces = (preserve_newline != 2);
			continue;
		}
		
		// Push current character into collector
		accum.push_back(c);
	} // End for
	
	if (!quote) throw Exception::Exception_nodebuilder("Input scalar is not in ' ' quotes");
	
	return accum;
}




std::string Builder::process_scalar_quotes_double(const std::string &input, const int preserve_newline) const
{
	std::string accum;
	std::string current_hex_value;
	
	bool escaped        = false;
	bool skiping_spaces = false;
	bool readhex        = false;
	
	if (input.front() != '\"' || input.back() != '\"') throw Exception::Exception_nodebuilder("Input scalar form is incorrect");
	
	for (const register char c : input)
	{
		// Skipping blank space before value start and in the beginning
		// of each line (if it is multiline scalar value)
		if (skiping_spaces && preserve_newline != 2)
		{
			if (c == ' ' || c == '\t') continue;
			else skiping_spaces = false;
		}
		
		// Escaped characters
		if (escaped)
		{
			switch (c)
			{
				case 'n': { accum.push_back('\n'); break; } // Line-break character
				case 't': { accum.push_back('\t'); break; } // Tabulator character
				case 's': { accum.push_back(' ');  break; } // Space
				case 'r': { accum.push_back('\r'); break; } // Carriage return
				case 'x': {                                 // Hex representation
					readhex = true;
					current_hex_value.clear();
					break;
				}
				// In all other cases just save escaped character
				default:
				{
					accum.push_back(c);
					break;
				}
			}
			escaped = false; // End escape
			continue;
		} // endif escaped
		else // If not escaped
		{
			// Begin and end of scalar (quotes)
			if (c == '\"') continue;
			
			// If function meets line-break
			if (c == '\n')
			{
				if (preserve_newline) accum.push_back('\n');
				skiping_spaces = (preserve_newline != 2);
				continue;
			}
		}
		
		if (readhex)
		{
			if (
				('0' <= c && c <= '9') ||
				('a' <= c && c <= 'f') ||
				('A' <= c && c <= 'F')
			) current_hex_value.push_back(c);
			else if (c == ';') // If ending symbol is valid (; in the end if hex symbol surrogate)
			{
				// Convert hex string into symbol
				std::vector<uint32_t> surrogate_vector = { (uint32_t)strtoul(current_hex_value.c_str(), NULL, 16) };
				current_hex_value = uint32_vector_to_string(surrogate_vector);
				for (char e : current_hex_value)
				{
					accum.push_back(e);
				}
				readhex = false;
			}
			// Else throw exception
			else
			{
				throw LSCL::Exception::Exception_nodebuilder("Symbol hex code in string is defined incorrectly (" + current_hex_value + "). Correct format is: \\x< hex code >;", filename_, get_line());
			}
			
			continue;
		}
		
		// If function meets escaping character
		// If it's not escaped, then start escape
		if (c == '\\') { escaped = !escaped; continue; }
		
		// Push current character into collector
		accum.push_back(c);
	} // End for
	
	// Erase white spaces in the end of unquoted string
	//if (!quote_double && !quote_single)
	//	while (answer.back() == ' ' || answer.back() == '\t' || answer.back() == '\n') answer.pop_back();
	
	return accum;
}


std::string Builder::process_acute_text(const std::string &input) const
{
	std::string accum;
	
	bool quote          = false;
//bool skiping_spaces = false;
	bool begin          = true;
	
	if (input.front() != '`' || input.back() != '`') throw Exception::Exception_nodebuilder("Input script form is incorrect");
	
	for (const register char c : input)
	{
// Skipping blank space before value start and in the beginning
// of each line (if it is multiline scalar value)
//if (skiping_spaces)
//{
//	if (c == ' ' || c == '\t') continue;
//	else skiping_spaces = false;
//}
		
		// Do not trigger on first quote
		if (begin)
		{
			begin = false;
			continue;
		}
		
		// One acute escapes another one (`` in config will be parsed as `)
		if (c == '`' && !begin)
		{
			if (quote) accum.push_back('`');
			quote = !quote;
			continue;
		}
		quote = false;
		
		// If function meets line-break
		if (c == '\n')
		{
			accum.push_back('\n');
//skiping_spaces = (preserve_newline != 2);
			continue;
		}
		
		// Push current character into collector
		accum.push_back(c);
	} // End for
	
	if (!quote) throw Exception::Exception_nodebuilder("Input script text is not in ` ` quotes");
	
	return accum;
}


void Builder::assign_links(void)
{
	for (auto e : linked_nodes_)
	{
		auto existing_link = links_.find(e->link_name);
		// If such link name was defined
		if (existing_link != links_.end())
		{
			std::cout << "** assigning link\n";
			e->linked = existing_link->second;
		}
		else throw LSCL::Exception::Exception_nodebuilder("Using undefined link \"" + e->link_name + "\"", filename_, get_line());
	}
	for (auto &e : references_)
	{
		e->linked = dig(e->address);
	}
}

void Builder::assign_includes(void)
{
	// Here we need timeout
	for (const auto &e : executed_list_)
	{
		if (pthread_join(e.thr, NULL))
		{
			throw Exception::Exception_nodebuilder("Unable to join script-processing thread", get_filename());
		}
		if (e.args.done)
		{
			e.target->attached = e.args.out;
		}
	}
}

void Builder::set_link(const std::string &linkname, Node_internal *n)
{
	links_.insert( { linkname, n } );
}
void Builder::use_link(Link *n)
{
	linked_nodes_.push_back(n);
}
void Builder::use_ref(Link *n)
{
	references_.push_back(n);
}


Node_internal* Builder::dig(const Link::lscl_path &path)
{
	register Node_internal *result = root_.get(); // result is tree root pointer
	for (const auto &e : path)
	{
		result = e.is_idx ?
			static_cast<List*>(result)->at(e.idx) :
			static_cast<Map* >(result)->at(e.text);
	}
	return result;
}

Builder::Executed& Builder::add_executed(void)
{
	executed_list_.push_back(Executed());
	return executed_list_.back();
}


} // Namespace Nodebuilder

} // Namespace LSCL
