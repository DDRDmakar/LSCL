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


%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {LSCL::Nodebuilder}
%define parser_class_name {LSCL_Parser}

%code requires {

	#include "../src/cpp/node_internal.hpp"

	namespace LSCL {
		namespace Nodebuilder
		{
			class Builder;
			class LSCL_Scanner;
		}
	}
	
// The following definitions is missing when %locations isn't used
#ifndef YY_NULLPTR
#if defined __cplusplus && 201103L <= __cplusplus
#define YY_NULLPTR nullptr
#else
#define YY_NULLPTR 0
#endif
#endif
	
}

%parse-param { LSCL_Scanner  &scanner }
%parse-param { Builder       &builder }

%code{
	#include <iostream>
	#include <cstdlib>
	#include <fstream>
	
	// include for all builder functions
	#include "nodebuilder.hpp"
	
	#undef  yylex
	#define yylex scanner.yylex
	
}

%define parse.assert

%define api.value.type variant
//%union {
//	std::string str;
//	LSCL::Node_internal node;
//}

%token                       END_OF_FILE
%token <LSCL::Node_internal> NODE
%token                       NEWLINE
%token                       SPACER
%token <std::string>         SCALAR_PLAINTEXT
%token <std::string>         SCALAR_DOUBLE_Q
%token <std::string>         SCALAR_SINGLE_Q

%type  <std::string>      scalar

%locations

%%

file: /*empty*/ | node ;

node
	: lscl_map 
	| lscl_list 
	| scalar 
	;

lscl_list: '[' lscl_list_body ']' ;

lscl_list_body
	: /*empty*/
	| node
	| lscl_list_body ',' node
	| lscl_list_body '\n' node
	;

lscl_map: '{' lscl_map_body '}' ;

kv_pair: scalar ':' node ;

lscl_map_body
	: /*empty*/
	| kv_pair
	| lscl_map_body ',' kv_pair
	| lscl_map_body '\n' kv_pair
	;

scalar
	: SCALAR_PLAINTEXT {
		$$ = LSCL::Nodebuilder::process_scalar_plaintext($1, false);
	}
	| '<' SCALAR_PLAINTEXT '>' {
		$$ = LSCL::Nodebuilder::process_scalar_plaintext($2, true);
	}
	| SCALAR_SINGLE_Q {
		$$ = LSCL::Nodebuilder::process_scalar_quotes_single($1, false);
	}
	| '<' SCALAR_SINGLE_Q '>' {
		$$ = LSCL::Nodebuilder::process_scalar_quotes_single($2, true);
	}
	| SCALAR_DOUBLE_Q {
		$$ = LSCL::Nodebuilder::process_scalar_quotes_double($1, false);
	}
	| '<' SCALAR_DOUBLE_Q '>' {
		$$ = LSCL::Nodebuilder::process_scalar_quotes_double($2, true);
	}
	;

%%


void LSCL::Nodebuilder::LSCL_Parser::error(const location_type &l, const std::string &err_message)
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
