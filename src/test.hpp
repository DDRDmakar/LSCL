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


#ifndef _H_LSCL_TEST
#define _H_LSCL_TEST

#include <list>
#include <string>

namespace LSCL
{
	
	namespace Test
	{
		struct Testcase
		{
			bool passed;
			std::string name;
			std::string error;
		};
		
		class Testblock
		{
		public:
			std::string name;
			std::list<Testcase> testcases;
			
			Testblock(const std::string &name);
			
			void add(const Testcase &tc);
			void add(const bool &passed, const std::string &name, const std::string &error = "");
			void add(const std::string &name, const std::string &error = "");
		};
		
		class Testdata
		{
		public:
			std::list<Testblock> testblocks;
			
			Testblock* add(const std::string &name);
			Testblock* add(const std::string &name, Testblock tb);
		};
		
		Testdata test_builder(void);
		
	} // Namespace test
	
} // Namespace LSCL

#endif // _H_LSCL_TEST
