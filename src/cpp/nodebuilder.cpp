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
	filename_(filename),
	root_created(false)
{
	if (filename.empty()) throw LSCL::Exception::Exception_nodebuilder("Empty file name is given", filename, get_line());
	std::ifstream in_file(filename.c_str());
	if(!in_file.good()) throw LSCL::Exception::Exception_nodebuilder("Error opening file", filename, get_line());
	build_tree(in_file);
	return;
}

Builder::Builder(std::istream &input) :
	root_created(false)
{
	std::cout << "Builder constructor\n";
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
}


size_t Builder::get_line(void) const
{
	return scanner_->lineno();
}


std::string process_scalar_plaintext(const std::string &input)
{
	std::cout << '|' << input << '|' << std::endl;
	return input;
}
std::string process_scalar_quotes_single(const std::string &input, const int preserve_newline) { std::cout << '|' << input << '|' << std::endl; return input; }
std::string process_scalar_quotes_double(const std::string &input, const int preserve_newline) { std::cout << '|' << input << '|' << std::endl; return input; }


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
