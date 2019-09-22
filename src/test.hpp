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
		
		class Testdata
		{
		public:
			std::list<Testcase> testcases;
			void add(Testcase tc);
			void add(bool passed, std::string name, std::string error = "");
			void add(std::string name, std::string error = "");
		};
		
	} // Namespace test
	
	bool test_builder(void);
	
} // Namespace LSCL

#endif // _H_LSCL_TEST
