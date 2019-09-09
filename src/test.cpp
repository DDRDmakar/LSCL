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


#include "../lscl/lscl.hpp"

#include <sstream>
#include <iostream>
#include <cstring>

namespace LSCL
{
	
	bool test_builder(void)
	{
		try
		{
		
			{
				// Single list
				std::string s = "[abcdefg]";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_list.size() != 1) return false;
				std::string s2 = builder.root.values_list[0].get<std::string>();
				if (strcmp(s2.c_str(), "abcdefg") != 0) return false;
			}
			{
				// Single map
				std::string s = "{key: val}";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_map.size() != 1) return false;
				std::string s2 = builder.root.values_map.find("key")->second.get<std::string>();
				if (strcmp(s2.c_str(), "val") != 0) return false;
			}
			
			{
				// List
				std::string s = "[abcdefg, xyz]";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_list.size() != 2) return false;
				std::string s2 = builder.root.values_list[0].get<std::string>();
				if (strcmp(s2.c_str(), "abcdefg") != 0) return false;
				s2 = builder.root.values_list[1].get<std::string>();
				if (strcmp(s2.c_str(), "xyz") != 0) return false;
			}
			{
				// Map
				std::string s = "{key: val, key2: val2}";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_map.size() != 2) return false;
				std::string s2 = builder.root.values_map.find("key")->second.get<std::string>();
				if (strcmp(s2.c_str(), "val") != 0) return false;
				s2 = builder.root.values_map.find("key2")->second.get<std::string>();
				if (strcmp(s2.c_str(), "val2") != 0) return false;
			}
			{
				// Map with quoted strings
				std::string s = "{key: \'val валъ\', key2: \"val2\\n\\t\"}";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_map.size() != 2) return false;
				std::string s2 = builder.root.values_map.find("key")->second.get<std::string>();
				if (strcmp(s2.c_str(), "val валъ") != 0) return false;
				s2 = builder.root.values_map.find("key2")->second.get<std::string>();
				if (strcmp(s2.c_str(), "val2\n\t") != 0) return false;
			}
			{
				// Empty list
				std::string s = "[]";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_list.size() != 0) return false;
			}
			{
				// Empty map
				std::string s = "{}";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_map.size() != 0) return false;
			}
			{
				// Empty input stream
				std::string s = "";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.type != LSCL::NODETYPE_NONE) return false;
			}
			{
				// No structure
				std::string s = "\n \n   \n\t\n   ";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.type != LSCL::NODETYPE_NONE) return false;
			}
			{
				// Scalar
				std::string s = "aaaa";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.type != LSCL::NODETYPE_SCALAR) return false;
			}
			{
				// Quoted scalar
				std::string s = "\"aaaa\"";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.type != LSCL::NODETYPE_SCALAR) return false;
			}
			{
				// \n should be ignored
				std::string s = "\"bbb\nb\"";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.type != LSCL::NODETYPE_SCALAR) return false;
				std::string s2 = builder.root.get<std::string>();
				if (strcmp(s2.c_str(), "bbbb") != 0) return false;
			}
			{
				// \n should be saved
				std::string s = "<\"bbb\nb\">";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.type != LSCL::NODETYPE_SCALAR) return false;
				std::string s2 = builder.root.get<std::string>();
				if (strcmp(s2.c_str(), "bbb\nb") != 0) return false;
			}
			{
				// \n should be ignored
				std::string s = "{key: \'va\nl\'}";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_map.size() != 1) return false;
				std::string s2 = builder.root.values_map.find("key")->second.get<std::string>();
				if (strcmp(s2.c_str(), "val") != 0) return false;
			}
			{
				// \n should be saved
				std::string s = "{key: <\'va\nl\'>}";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_map.size() != 1) return false;
				std::string s2 = builder.root.values_map.find("key")->second.get<std::string>();
				if (strcmp(s2.c_str(), "va\nl") != 0) return false;
			}
			
			{
				// Nested list
				std::string s = "[abcdefg, [xyz]]";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_list.size() != 2) return false;
				std::string s2 = builder.root.values_list[0].get<std::string>();
				if (strcmp(s2.c_str(), "abcdefg") != 0) return false;
				if (builder.root.values_list[1].type != LSCL::NODETYPE_LIST) return false;
				s2 = builder.root.values_list[1].values_list[0].get<std::string>();
				if (strcmp(s2.c_str(), "xyz") != 0) return false;
			}
			{
				// Nested map
				std::string s = "{key: val, key2: {key3: val3}}";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_map.size() != 2) return false;
				std::string s2 = builder.root.values_map.find("key")->second.get<std::string>();
				if (strcmp(s2.c_str(), "val") != 0) return false;
				if (builder.root.values_map.find("key2")->second.type != LSCL::NODETYPE_MAP) return false;
				if (builder.root.values_map.find("key2")->second.size() != 1) return false;
				s2 = builder.root.values_map.find("key2")->second.values_map.find("key3")->second.get<std::string>();
				if (strcmp(s2.c_str(), "val3") != 0) return false;
			}
			
			{
				// Nested single list
				std::string s = "[[xyz]]";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_list.size() != 1) return false;
				if (builder.root.values_list[0].type != LSCL::NODETYPE_LIST) return false;
				std::string s2 = builder.root.values_list[0].values_list[0].get<std::string>();
				if (strcmp(s2.c_str(), "xyz") != 0) return false;
			}
			{
				// Nested single map
				std::string s = "{key2:{key3: val3}}";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_map.size() != 1) return false;
				if (builder.root.values_map.find("key2")->second.type != LSCL::NODETYPE_MAP) return false;
				if (builder.root.values_map.find("key2")->second.size() != 1) return false;
				std::string s2 = builder.root.values_map.find("key2")->second.values_map.find("key3")->second.get<std::string>();
				if (strcmp(s2.c_str(), "val3") != 0) return false;
			}
			
			{
				// Multiline nested single map
				std::string s = 
				"{                  \n"
				"	key2:           \n"
				"	{               \n"
				"		key3: val3  \n"
				"	}               \n"
				"}                  \n";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.values_map.size() != 1) return false;
				if (builder.root.values_map.find("key2")->second.type != LSCL::NODETYPE_MAP) return false;
				if (builder.root.values_map.find("key2")->second.size() != 1) return false;
				std::string s2 = builder.root.values_map.find("key2")->second.values_map.find("key3")->second.get<std::string>();
				if (strcmp(s2.c_str(), "val3") != 0) return false;
			}
			
			{
				// Hex code
				std::string s = "aaaa\\x55;";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.type != LSCL::NODETYPE_SCALAR) return false;
				std::string s2 = builder.root.get<std::string>();
				//std::cout << '|' << s2 << '|' << std::endl;
				if (strcmp(s2.c_str(), "aaaa\x55") != 0) return false;
			}
			
			{
				// Hex code
				std::string s = "\\x0412;\\x0430;\\x0440;\\x0435;\\x043D;\\x044c;\\x0435;";
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				if (builder.root.type != LSCL::NODETYPE_SCALAR) return false;
				std::string s2 = builder.root.get<std::string>();
				if (strcmp(s2.c_str(), "Варенье") != 0) return false;
			}
		}
		catch (LSCL::Exception::Exception_nodebuilder &e)
		{
			std::cout << e.what() << std::endl;
			return false;
		}
		catch (LSCL::Exception::Exception &e)
		{
			std::cout << e.what() << std::endl;
			return false;
		}
		
		return true;
	}
	
} // namespace LSCL

