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



#include <lscl/lscl.hpp>
#include "test.hpp"

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
	);                                                                 \
	if (!tb->testcases.back().passed) goto endtest;                    \
	
	
// S1 - current, S2 - expected
#define CMP_NUM(n1, n2, name)                                                              \
	tb->add(                                                                               \
		(n1) == (n2),                                                                      \
		(name),                                                                            \
		"Value is " + std::to_string(n1) + ", but " + std::to_string(n2) + " is expected"  \
	);                                                                                     \
	if (!tb->testcases.back().passed) goto endtest;                                        \
	
	
#define CMP_TYPE(t1, t2, name)                                                           \
	tb->add(                                                                             \
		(t1) == (t2),                                                                    \
		(name),                                                                          \
		"Type is " + std::to_string(t1) + ", but " + std::to_string(t2) + " is expected" \
	);                                                                                   \
	if (!tb->testcases.back().passed) goto endtest;                                      \
	
#define TEST_BOOL(cond, name)  \
	tb->add(                   \
		(cond),                \
		(name),                \
		""                     \
	);                         \
	if (!(cond)) goto endtest; \
	
	
	
	Testdata test_builder(void)
	{
		Testdata data;
		Testblock *tb;
		
		try
		{
			{
				// Single list
				std::string s = "[abcdefg]";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				TEST_BOOL(builder.root_->type != LSCL::NODETYPE_NONE, "Root is NULL");
				CMP_NUM(builder.root_->size(), 1, "List size");
				std::string s2 = LSCL::Node(builder.root_->at(0)).get<std::string>();
				CMP_TEXT(s2, "abcdefg", "List contents");
			}
			{
				// Single map
				std::string s = "{key: val}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root_->size(), 1, "Map size");
				std::string s2 = Node(builder.root_->at("key")).get<std::string>();
				CMP_TEXT(s2, "val", "Map contents");
			}
			
			
			
			{
				// List
				std::string s = "[,abcdefg, xyz,]";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root_->size(), 2, "List size");
				std::string s2 = Node(builder.root_->at(0)).get<std::string>();
				CMP_TEXT(s2, "abcdefg", "List contents");
				s2 = Node(builder.root_->at(1)).get<std::string>();
				CMP_TEXT(s2, "xyz", "List contents");
			}
			{
				// Map
				std::string s = "{key: val, key2: val2}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root_->size(), 2, "Map size");
				std::string s2 = Node(builder.root_->at("key")).get<std::string>();
				CMP_TEXT(s2, "val", "Value 1");
				s2 = Node(builder.root_->at("key2")).get<std::string>();
				CMP_TEXT(s2, "val2", "Value 2");
			}
			
			
			
			
			{
				// Map with quoted strings
				std::string s = "{key: \'val валъ\', key2: \"val2\\n\\t\"}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root_->size(), 2, "Map size");
				std::string s2 = Node(builder.root_->at("key")).get<std::string>();
				CMP_TEXT(s2, "val валъ", "First value in map");
				s2 = Node(builder.root_->at("key2")).get<std::string>();
				CMP_TEXT(s2, "val2\n\t", "Second value in map");
			}
			
			
			
			
			{
				// Empty list
				std::string s = "[]";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root_->size(), 0, "List size");
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_LIST, "Root node type");
			}
			{
				// Empty map
				std::string s = "{}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root_->size(), 0, "Map size");
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_MAP, "Root node type");
			}
			{
				// Empty input stream
				std::string s = "";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_NONE, "Root node type");
			}
			{
				// No structure
				std::string s = "\n \n   \n\t\n   ";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_NONE, "Root node type");
			}
			
			
			{
				// Scalar
				std::string s = "aaaa";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = Node(builder.root_).get<std::string>();
				CMP_TEXT(s2, s, "Scalar value");
			}
			{
				// Scalar
				std::string s = "аАяЯ";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = Node(builder.root_).get<std::string>();
				CMP_TEXT(s2, s, "Scalar value");
			}
			{
				// Quoted scalar
				std::string s = "\"aaaa\"";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = static_cast<LSCL::Scalar*>(builder.root_.get())->get<std::string>();
				CMP_TEXT(s2, "aaaa", "Scalar value");
			}
			
			
			
			
			{
				// \n should be ignored
				std::string s = "\"bbb\nb\"";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = Node(builder.root_).get<std::string>();
				CMP_TEXT(s2, "bbbb", "Scalar value");
			}
			{
				// \n should be saved
				std::string s = "<\"bbb\nb\">";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = Node(builder.root_).get<std::string>();
				CMP_TEXT(s2, "bbb\nb", "Scalar value");
			}
			{
				// \n should be ignored
				std::string s = "{key: \'va\nl\'}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root_->size(), 1, "Map size");
				std::string s2 = Node(builder.root_->at("key")).get<std::string>();
				CMP_TEXT(s2, "val", "Value in map");
			}
			{
				// \n should be saved
				std::string s = "{key: <\'va\nl\'>}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root_->size(), 1, "Map size");
				std::string s2 = Node(builder.root_->at("key")).get<std::string>();
				CMP_TEXT(s2, "va\nl", "Value in map");
			}
			
			
			
			
			{
				// Nested list
				std::string s = "[abcdefg, [xyz]]";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_LIST, "External list type");
				CMP_NUM(builder.root_->size(), 2, "External list size");
				std::string s2 = Node(builder.root_->at(0)).get<std::string>();
				CMP_TEXT(s2, "abcdefg", "External list value");
				CMP_TYPE(builder.root_->at(1)->type, LSCL::NODETYPE_LIST, "Internal list type");
				CMP_NUM(builder.root_->at(1)->size(), 1, "Internal list size");
				s2 = Node(builder.root_->at(1)->at(0)).get<std::string>();
				CMP_TEXT(s2, "xyz", "Internal list value");
			}
			{
				// Nested map
				std::string s = "{key: '''''', key2: {key3: val3}}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_MAP, "External map type");
				CMP_NUM(builder.root_->size(), 2, "External map size");
				std::string s2 = Node(builder.root_->at("key")).get<std::string>();
				CMP_TEXT(s2, "\'\'", "External map value");
				CMP_TYPE(builder.root_->at("key2")->type, LSCL::NODETYPE_MAP, "Internal map type");
				CMP_NUM(builder.root_->at("key2")->size(), 1, "Internal map size");
				s2 = Node(builder.root_->at("key2")->at("key3")).get<std::string>();
				CMP_TEXT(s2, "val3", "Internal map value");
			}
			{
				// Nested single list
				std::string s = "[[xyz]]";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root_->size(), 1, "External list size");
				CMP_TYPE(builder.root_->at(0)->type, LSCL::NODETYPE_LIST, "Internal list type");
				CMP_NUM(builder.root_->at(0)->size(), 1, "Internal list size");
				std::string s2 = Node(builder.root_->at(0)->at(0)).get<std::string>();
				CMP_TEXT(s2, "xyz", "Internal list value");
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
				CMP_NUM(builder.root_->size(), 1, "External map size");
				CMP_TYPE(builder.root_->at("key2")->type, LSCL::NODETYPE_MAP, "Internal map type");
				CMP_NUM(builder.root_->at("key2")->size(), 1, "Internal map size");
				std::string s2 = Node(builder.root_->at("key2")->at("key3")).get<std::string>();
				CMP_TEXT(s2, "val3", "Internal map value");
			}
			
			
			
			{
				// Hex code
				std::string s = "aaaa\\x55;";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = Node(builder.root_).get<std::string>();
				CMP_TEXT(s2, "aaaa\x55", "Scalar value");
			}
			{
				// Hex code
				std::string s = "\"aaaa\\x55;\"";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = Node(builder.root_).get<std::string>();
				CMP_TEXT(s2, "aaaa\x55", "Scalar value");
			}
			{
				// Hex code
				std::string s = "\\x0412;\\x0430;\\x0440;\\x0435;\\x043D;\\x044c;\\x0435;";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_TYPE(builder.root_->type, LSCL::NODETYPE_SCALAR, "Root node type");
				std::string s2 = Node(builder.root_).get<std::string>();
				CMP_TEXT(s2, "Варенье", "Scalar value");
			}
			
			
			
			
			{
				// link creation (quoted)
				std::string s = "{az: &'link' val}";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root_->size(), 1, "Map size");
				std::string s2 = Node(builder.root_->at("az")).get<std::string>();
				CMP_TEXT(s2, "val", "Value in map");
				CMP_NUM(builder.links_.size(), 1, "Created links number");
				CMP_TYPE(builder.links_["link"]->type, LSCL::NODETYPE_SCALAR, "Linked node type");
			}
			{
				// link creation (single word)
				std::string s = 
				"{\n"
				",	az: &link val     \n"
				",	as : *link        \n"
				",	as2: *\"link\"    \n"
				",	as3: *\"li\nnk\"  \n"
				",	dedy: vaevale     \n"
				",	ke: \"ge\"       ,\n"
				"}\n";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root_->size(), 6, "Map size");
				std::string s2 = Node(builder.root_)["az"].get<std::string>();
				CMP_TEXT(s2, "val", "az value");
				CMP_NUM(builder.links_.size(), 1, "Created links number");
				CMP_TYPE(builder.links_["link"]->type, LSCL::NODETYPE_SCALAR, "linked node type");
				CMP_TYPE(builder.root_->at("as")->type, LSCL::NODETYPE_LINK, "linking node type");
				LSCL::Link &l = *static_cast<LSCL::Link*>(builder.root_->at("as"));
				TEST_BOOL(l.linked != nullptr, "Link is NULL, pointer is expected");
				s2 = Node(l.linked).get<std::string>();
				CMP_TEXT(s2, "val", "as node value");
				CMP_TYPE(builder.root_->at("as2")->type, LSCL::NODETYPE_LINK, "as2 node type");
				
				CMP_TYPE(builder.root_->at("as3")->type, LSCL::NODETYPE_LINK, "as3 node type");
				
				s2 = Node(builder.root_)["ke"].get<std::string>();
				CMP_TEXT(s2, "ge", "ke value");
			}
			{
				std::string s = 
				"{                                                                     \n"
				",    kek:    kok                                                      \n"
				",    lel:    {lel: {lel: {lel:lol     }}   }                          \n"
				",    lesley: [one, two, three, {\"ключ\": \"значение\"}, five]        \n"
				",    КЛЮЧИК   :    *(lesley.[3].ключ)                                 \n"
				",    ключик0  :    *(lesley.[1])                                      \n"
				",    ключик1  :    *(\"kek\")                                         \n"
				",    ключик2  :    *(kek)                                             \n"
				",    ключик3  :    *(lel.lel.lel.lel)                                 \n"
				",    ключик4  :    *('lel'.lel.lel.lel)                               \n"
				",    ключик5  :    *(lel.'lel'.lel.lel)                               \n"
				",    ключик6  :    *(lel.lel.lel.'lel')                               \n"
				"}                                                                     \n";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root_->size(), 11, "Map size");
				CMP_NUM(builder.references_.size(), 8, "Number of references");
				CMP_TYPE(builder.root_->at("КЛЮЧИК")->type, LSCL::NODETYPE_LINK, "element КЛЮЧИК type");
				TEST_BOOL( ((Link*)(builder.root_->at("КЛЮЧИК")))->linked != nullptr, "linked node КЛЮЧИК is NULL" );
				CMP_TEXT(Node(builder.root_)["КЛЮЧИК"].get<std::string>(), "значение", "element КЛЮЧИК linked value");
				CMP_TEXT(Node(builder.root_)["ключик0"].get<std::string>(), "two", "element ключик0 linked value");
				CMP_TEXT(Node(builder.root_)["ключик1"].get<std::string>(), "kok", "element ключик1 linked value");
				CMP_TEXT(Node(builder.root_)["ключик2"].get<std::string>(), "kok", "element ключик2 linked value");
				CMP_TEXT(Node(builder.root_)["ключик3"].get<std::string>(), "lol", "element ключик3 linked value");
				CMP_TEXT(Node(builder.root_)["ключик4"].get<std::string>(), "lol", "element ключик4 linked value");
				CMP_TEXT(Node(builder.root_)["ключик5"].get<std::string>(), "lol", "element ключик5 linked value");
				CMP_TEXT(Node(builder.root_)["ключик6"].get<std::string>(), "lol", "element ключик6 linked value");
			}
			{
				std::string s = 
				"[                                                                   \n"
				"    `                                                               \n"
				"import time\n"
				"print('[')\n"
				"for i in range(10):\n"
				"	print('%d, ' % i**2)\n"
				"print(\"'``'\")\n"
				"print(']')\n"
				"time.sleep(2)\n"
				"    `                                                               \n"
				"]                                                                   \n";
				tb = data.add(s);
				std::stringstream ss(s);
				LSCL::Nodebuilder::Builder builder(ss);
				CMP_NUM(builder.root_->size(), 1, "External list size");
				CMP_NUM(builder.getroot()[0].size(), 11, "Internal list size");
			}
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
		
		endtest:   ;
		return data;
	}
	
} // Namespace Test
	
} // namespace LSCL

