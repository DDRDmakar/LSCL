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

#ifndef _H_LSCL_BUILDER
#define _H_LSCL_BUILDER

#include <string>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <memory>

#include "node_internal.hpp"

#include "../flexbison/lscl_scanner.hpp"
#include "../../build/lscl_parser.tab.hh"

#include "test.hpp"

using namespace std;

namespace LSCL
{
	
	namespace Nodebuilder
	{
		
		enum NODEWAY
		{
			NODEWAY_MAP    = 1,
			NODEWAY_LIST   = 2,
			NODEWAY_SCALAR = 3,
			NODEWAY_KEY,
			NODEWAY_COMMA_MAP,
			NODEWAY_COMMA_LIST
		};
		
		/*
		* It builds a tree of nodes from text stream
		* 
		*/
		class Builder
		{
			
		private:
			
			LSCL_Parser  *parser_  = nullptr;
			LSCL_Scanner *scanner_ = nullptr;
			
			// Friend function to perform tests
			friend LSCL::Test::Testdata LSCL::Test::test_builder(void);
			
			
			std::string filename_; // Name of processed file (empty if no file)
			std::unordered_map<std::string, Node_internal*> links_; // Named links to nodes
			std::unordered_map<std::string, Node_internal*> linked_nodes_; // Nodes which should be linked by named links
			
			void assign_links(void); // Assign links to link names after all objects are created
			void build_tree(std::istream &input);
			size_t get_line(void) const;
			
		public:
			explicit Builder(std::istream& input);
			explicit Builder(const std::string &filename = "");
			virtual ~Builder();
			
			Node_internal *workpoint; // Pointer to current container
			bool root_created; // If root node was defined and created yet
			std::stack<NODEWAY> nodestack; // Stack of the hierarchy we are digging into
			
			Node_internal root; // Root of node tree
		};
		
		
		std::string process_scalar_plaintext(    const std::string &input);
		// 0 - nothing, 1 - preserve newlines, 2 - preserve everything
		std::string process_scalar_quotes_single(const std::string &input, const int preserve_newline);
		std::string process_scalar_quotes_double(const std::string &input, const int preserve_newline);
		
	} // Namespace Nodebuilder

} // Namespace LSCL

#endif // _H_LSCL_NODEBUILDER
