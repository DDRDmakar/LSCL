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

#include "node_internal.hpp"
#include "node.hpp"
#include "exception.hpp"

#include "../flexbison/lscl_scanner.hpp"
#include "../../build/lscl_parser.tab.hh"

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
			
		public:
			
			enum EXECUTED_TYPE
			{
				EXECUTED_TYPE_NONE,
				EXECUTED_TYPE_SCRIPT,
				EXECUTED_TYPE_INCLUDE
			};
			
			struct Executed_args
			{
				std::string   in;
				Node_internal *out;
				bool done; // If parsing is done
				Exception::Exception_nodebuilder e; // If done and *out is nullptr we throw this exception
			};
			
			struct Executed
			{
				EXECUTED_TYPE type;
				pthread_t thr;
				Attached *target;
				Executed_args args;
			};
			
		private:
			
			LSCL_Parser  *parser_  = nullptr;
			LSCL_Scanner *scanner_ = nullptr;
			
			// Friend function to perform tests
			friend LSCL::Test::Testdata LSCL::Test::test_builder(void);
			
			std::unique_ptr<Node_internal> root_; // Root of node tree
			std::string filename_; // Name of processed file (empty if no file)
			std::unordered_map<std::string, Node_internal*> links_; // Named links to nodes
			std::unordered_map<std::string, Link*> linked_nodes_;   // Nodes which should be linked by named links
			std::list<Link*> references_; // Nodes which should be linked by references (absolute path)
			std::list<Executed> executed_list_; // Information about 
			
			void assign_links(void); // Assign links to link names after all objects are created
			void build_tree(std::istream &input);
			size_t get_line(void) const;
			Node_internal* dig(const Link::lscl_path &path); // Get pointer to node referenced by given path from root
			void assign_includes(void);
			
		public:
			
			explicit Builder(std::istream& input);
			explicit Builder(const std::string &filename = "");
			virtual ~Builder(void);
			
			void setroot(Node_internal *newroot);
			const std::string& get_filename(void) const;
			Node getroot(void) const;
			Node_internal* getroot_internal(void) const;
			Node_internal* release_root(void);
			
			void set_link(const std::string &linkname, Node_internal *n);
			void use_link(const std::string &linkname, Link *n);
			void use_ref(Link *n);
			Executed& add_executed(void);
			
			std::string process_scalar_plaintext(const std::string &input) const;
			// 0 - nothing, 1 - preserve newlines, 2 - preserve everything
			std::string process_scalar_quotes_single(const std::string &input, const int preserve_newline) const;
			std::string process_scalar_quotes_double(const std::string &input, const int preserve_newline) const;
			std::string process_acute_text(const std::string &input) const;
		};
		
	} // Namespace Nodebuilder

} // Namespace LSCL

#endif // _H_LSCL_BUILDER
