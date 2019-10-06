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
namespace Test
{
	
	Testblock::Testblock(const std::string &name) : name(name)
	{
	}
	
	Testblock* Testdata::add(const std::string &name)
	{
		testblocks.push_back(Testblock(name));
		return &testblocks.back();
	}
	Testblock* Testdata::add(const std::string &name, Testblock tb)
	{
		tb.name = name;
		testblocks.push_back(tb);
		return &testblocks.back();
	}
	
	
	void Testblock::add(const Testcase &tc)
	{
		testcases.push_back(tc);
	}
	void Testblock::add(const bool &passed, const std::string &name, const std::string &error)
	{
		testcases.push_back(Testcase{passed, name, error});
	}
	void Testblock::add(const std::string &name, const std::string &error)
	{
		testcases.push_back(Testcase{error.empty(), name, error});
	}
	
	
	
	
// S1 - current, S2 - expected
#define CMP_TEXT(s1, s2, name)                                         \
	tb->add(                                                           \
		std::string(s1).compare(s2) == 0,                              \
		(name),                                                        \
		"Value is |" + (s1) + "|, but |" + (s2) + "| is expected"      \
	)                                                                  \
	
	
// S1 - current, S2 - expected
#define CMP_NUM(n1, n2, name)                                                              \
	tb->add(                                                                               \
		(n1) == (n2),                                                                      \
		(name),                                                                            \
		"Value is " + std::to_string(n1) + ", but " + std::to_string(n2) + " is expected"  \
	)                                                                                      \
	
	
#define CMP_TYPE(t1, t2, name)                                                           \
	tb->add(                                                                             \
		(t1) == (t2),                                                                    \
		(name),                                                                          \
		"Type is " + std::to_string(t1) + ", but " + std::to_string(t2) + " is expected" \
	)                                                                                    \
	
	
	
	Testdata test_builder(void)
	{
		Testdata data;
		Testblock *tb;
		
		try
		{/*
			{
				// Single list
				std::string s = "[abcdefg]";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_list.size(), 1, "List size");
				std::string s2 = builder.root->values_list[0].get<std::string>();
				CMP_TEXT(s2, "abcdefg", "List contents");
			}
			{
				// Single map
				std::string s = "{key: val}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_map.size(), 1, "Map size");
				std::string s2 = builder.root->values_map.find("key")->second.get<std::string>();
				CMP_TEXT(s2, "val", "Map contents");
			}
			
			
			
			
			{
				// List
				std::string s = "[abcdefg, xyz]";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_list.size(), 2, "List size");
				std::string s2 = builder.root->values_list[0].get<std::string>();
				CMP_TEXT(s2, "abcdefg", "List contents");
				s2 = builder.root->values_list[1].get<std::string>();
				CMP_TEXT(s2, "xyz", "List contents");
			}
			{
				// Map
				std::string s = "{key: val, key2: val2}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_map.size(), 2, "Map size");
				std::string s2 = builder.root->values_map.find("key")->second.get<std::string>();
				CMP_TEXT(s2, "val", "Value 1");
				s2 = builder.root->values_map.find("key2")->second.get<std::string>();
				CMP_TEXT(s2, "val2", "Value 2");
			}
			
			
			
			
			{
				// Map with quoted strings
				std::string s = "{key: \'val валъ\', key2: \"val2\\n\\t\"}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_map.size(), 2, "Map size");
				std::string s2 = builder.root->values_map.find("key")->second.get<std::string>();
				CMP_TEXT(s2, "val валъ", "First value in map");
				s2 = builder.root->values_map.find("key2")->second.get<std::string>();
				CMP_TEXT(s2, "val2\n\t", "Second value in map");
			}
			
			
			
			
			{
				// Empty list
				std::string s = "[]";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_list.size(), 0, "List size");
				CMP_TYPE(builder.root->type, LSCL::NODETYPE_LIST, "Root node type");
			}
			{
				// Empty map
				std::string s = "{}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_map.size(), 0, "Map size");
				CMP_TYPE(builder.root->type, LSCL::NODETYPE_MAP, "Root node type");
			}
			{
				// Empty input stream
				std::string s = "";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root->type, LSCL::NODETYPE_NONE, "Root node type");
			}
			{
				// No structure
				std::string s = "\n \n   \n\t\n   ";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root->type, LSCL::NODETYPE_NONE, "Root node type");
			}
			
			
			
			
			{
				// Scalar
				std::string s = "aaaa";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = builder.root->get<std::string>();
				CMP_TEXT(s2, s, "Scalar value");
			}
			{
				// Scalar
				std::string s = "аАяЯ";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = builder.root->get<std::string>();
				CMP_TEXT(s2, s, "Scalar value");
			}
			{
				// Quoted scalar
				std::string s = "\"aaaa\"";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = builder.root->get<std::string>();
				CMP_TEXT(s2, "aaaa", "Scalar value");
			}
			
			
			
			
			{
				// \n should be ignored
				std::string s = "\"bbb\nb\"";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = builder.root->get<std::string>();
				CMP_TEXT(s2, "bbbb", "Scalar value");
			}
			{
				// \n should be saved
				std::string s = "<\"bbb\nb\">";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = builder.root->get<std::string>();
				CMP_TEXT(s2, "bbb\nb", "Scalar value");
			}
			{
				// \n should be ignored
				std::string s = "{key: \'va\nl\'}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_map.size(), 1, "Map size");
				std::string s2 = builder.root->values_map.find("key")->second.get<std::string>();
				CMP_TEXT(s2, "val", "Value in map");
			}
			{
				// \n should be saved
				std::string s = "{key: <\'va\nl\'>}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_map.size(), 1, "Map size");
				std::string s2 = builder.root->values_map.find("key")->second.get<std::string>();
				CMP_TEXT(s2, "va\nl", "Value in map");
			}
			{
				// Triangle brackets without quotes
				std::string s = "{key: <va\nl>}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_map.size(), 1, "Map size");
				std::string s2 = builder.root->values_map.find("key")->second.get<std::string>();
				CMP_TEXT(s2, "va\nl", "Value in map");
			}
			
			
			
			
			{
				// Nested list
				std::string s = "[abcdefg, [xyz]]";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_list.size(), 2, "External list size");
				std::string s2 = builder.root->values_list[0].get<std::string>();
				CMP_TEXT(s2, "abcdefg", "External list value");
				CMP_TYPE(builder.root->values_list[1].type, LSCL::NODETYPE_LIST, "Internal list type");
				CMP_NUM(builder.root->values_list[1].values_list.size(), 1, "Internal list size");
				s2 = builder.root->values_list[1].values_list[0].get<std::string>();
				CMP_TEXT(s2, "xyz", "Internal list value");
			}
			{
				// Nested map
				std::string s = "{key: val, key2: {key3: val3}}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_map.size(), 2, "External map size");
				std::string s2 = builder.root->values_map.find("key")->second.get<std::string>();
				CMP_TEXT(s2, "val", "External map value");
				CMP_TYPE(builder.root->values_map["key2"].type, LSCL::NODETYPE_MAP, "Internal map type");
				CMP_NUM(builder.root->values_map["key2"].values_map.size(), 1, "Internal map size");
				s2 = builder.root->values_map.find("key2")->second.values_map.find("key3")->second.get<std::string>();
				CMP_TEXT(s2, "val3", "Internal map value");
			}
			{
				// Nested single list
				std::string s = "[[xyz]]";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_list.size(), 1, "External list size");
				CMP_TYPE(builder.root->values_list[0].type, LSCL::NODETYPE_LIST, "Internal list type");
				CMP_NUM(builder.root->values_list[0].values_list.size(), 1, "Internal list size");
				std::string s2 = builder.root->values_list[0].values_list[0].get<std::string>();
				CMP_TEXT(s2, "xyz", "Internal list value");
			}
			{
				// Nested single map
				std::string s = "{key2:{key3: val3}}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_map.size(), 1, "External map size");
				CMP_TYPE(builder.root->values_map["key2"].type, LSCL::NODETYPE_MAP, "Internal map type");
				CMP_NUM(builder.root->values_map["key2"].values_map.size(), 1, "Internal map size");
				std::string s2 = builder.root->values_map.find("key2")->second.values_map.find("key3")->second.get<std::string>();
				CMP_TEXT(s2, "val3", "Internal map value");
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
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_map.size(), 1, "External map size");
				CMP_TYPE(builder.root->values_map["key2"].type, LSCL::NODETYPE_MAP, "Internal map type");
				CMP_NUM(builder.root->values_map["key2"].values_map.size(), 1, "Internal map size");
				std::string s2 = builder.root->values_map.find("key2")->second.values_map.find("key3")->second.get<std::string>();
				CMP_TEXT(s2, "val3", "Internal map value");
			}
			
			
			
			
			{
				// Hex code
				std::string s = "aaaa\\x55;";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = builder.root->get<std::string>();
				CMP_TEXT(s2, "aaaa\x55", "Scalar value");
			}
			{
				// Hex code
				std::string s = "\\x0412;\\x0430;\\x0440;\\x0435;\\x043D;\\x044c;\\x0435;";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = builder.root->get<std::string>();
				CMP_TEXT(s2, "Варенье", "Scalar value");
			}
			
			
			
			
			{
				// link creation (quoted)
				std::string s = "{az: &'link' val}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_map.size(), 1, "Map size");
				std::string s2 = builder.root->values_map["az"].get<std::string>();
				CMP_TEXT(s2, "val", "Value in map");
				CMP_NUM(builder.links_.size(), 1, "Created links number");
				CMP_TYPE(builder.links_["link"]->type, LSCL::NODETYPE_SCALAR, "Linked node type");
			}
			
			{
				// link creation (single word)
				std::string s = 
				"{\n"
				"	az: &link val, \n"
				"	as : *link,    \n"
				"	as2: *\"link\",\n"
				"	as3: *<link> , \n"
				"	dedy: vaevale  \n"
				"	<ke>: \"ge\""
				"}\n";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root->values_map.size(), 5, "Map size");
				std::string s2 = builder.root->values_map["az"].get<std::string>();
				CMP_TEXT(s2, "val", "az value");
				CMP_NUM(builder.links_.size(), 1, "Created links number");
				CMP_TYPE(builder.links_["link"]->type, LSCL::NODETYPE_SCALAR, "linked node type");
				CMP_TYPE(builder.root->values_map["as"].type, LSCL::NODETYPE_LINK, "linking node type");
				s2 = builder.root->values_map["as"].linked->get<std::string>();
				CMP_TEXT(s2, "val", "as node value");
				CMP_TYPE(builder.root->values_map["as2"].type, LSCL::NODETYPE_LINK, "as2 node type");
				
				CMP_TYPE(builder.root->values_map["as3"].type, LSCL::NODETYPE_LINK, "as3 type");
				
				s2 = builder.root->values_map["ke"].get<std::string>();
				CMP_TEXT(s2, "ge", "ke value");
			}
			*/
		}
		catch (LSCL::Exception::Exception_nodebuilder &e)
		{
			tb = data.add("EXCEPTION");
			tb->add(false, "EXCEPTION", e.what());
		}
		catch (LSCL::Exception::Exception &e)
		{
			tb = data.add("EXCEPTION");
			tb->add(false, "EXCEPTION", e.what());
		}
		
		return data;
	}
	
} // Namespace Test
	
} // namespace LSCL
