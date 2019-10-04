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


#ifndef _H_LSCL_SCANNER
#define _H_LSCL_SCANNER

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "lscl_parser.tab.hh"
#include "location.hh"

namespace LSCL
{
namespace Nodebuilder
{
	class LSCL_Scanner : public yyFlexLexer
	{
	public:
	
		LSCL_Scanner(std::istream *in) : yyFlexLexer(in)
		{
		};
		virtual ~LSCL_Scanner()
		{
		};
		
		// Get rid of override virtual function warning
		using FlexLexer::yylex;
		
		virtual int yylex(
			LSCL_Parser::semantic_type* const lval, 
			LSCL_Parser::location_type *location
		);
		// YY_DECL defined in lscl_lexer.l
		// Method body created by flex in lscl_lexer.yy.cc
		
	private:
		// yyval ptr
		LSCL_Parser::semantic_type *yylval = nullptr;
	};
	
} // Namespace Nodebuilder
} // Namespace LSCL

#endif // _H_LSCL_SCANNER
