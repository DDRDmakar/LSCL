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

#include "../lscl/lscl.hpp"
#include "stream.hpp"
#include "node_internal.hpp"

#include "lscl_scanner.hpp"
#include "lscl_parser.tab.hh"

#include "test.hpp"

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
			
		private:
			
			enum NODEWAY
			{
				NODEWAY_MAP    = 1,
				NODEWAY_LIST   = 2,
				NODEWAY_SCALAR = 3,
				NODEWAY_KEY,
				NODEWAY_COMMA_MAP,
				NODEWAY_COMMA_LIST
			};
			
			// Friend function to perform tests
			friend LSCL::Test::Testdata LSCL::Test::test_builder(void);
			friend std::string get_nodeway_name(NODEWAY type);
			
			Stream ss_;                     // Characters stream to read from
			std::stack<NODEWAY> nodestack_; // Stack of the hierarchy we are digging into
			std::string filename_;          // Name of parsed file (or empty if we parse string)
			std::unordered_map<std::string, Node_internal*> links_; // Named links to nodes
			std::unordered_map<std::string, Node_internal*> linked_nodes_; // Nodes which should be linked by named links
			
			std::pair<std::string, bool> process_scalar(void); // Processing of scalar value
			std::pair<std::string, bool> process_single_word(void); // Processing plain text word without escaped characters quotes, spaces and line-breaks
			void process_directive(Node_internal *workpoint); // Processing of directive
			void assign_links(void); // Assign links to link names after all objects are created
			void build_tree(void);
			
		public:
			
			explicit Builder(std::istream& input, const std::string &filename = "");
			
			// If the parser has some valid input to be read
			// explicit operator bool() const;
			
			std::shared_ptr<Node_internal> root; // Root of node tree
		};
		
	} // Namespace Nodebuilder

} // Namespace LSCL

#endif // _H_LSCL_NODEBUILDER
