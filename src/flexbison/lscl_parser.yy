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
%token                       NEWLINE
%token                       SPACER
%token <std::string>         SCALAR_PLAINTEXT
%token <std::string>         SCALAR_DOUBLE_Q
%token <std::string>         SCALAR_SINGLE_Q

%type  <Node_internal>                             file
%type  <Node_internal>                             scalar
%type  <Node_internal>                             node
%type  <Node_internal>                             node_2
%type  <std::shared_ptr<Node_internal::lscl_map>>  lscl_map_body
%type  <Node_internal>                             lscl_map
%type  <std::shared_ptr<Node_internal::lscl_list>> lscl_list_body
%type  <Node_internal>                             lscl_list

%locations

%%

// Whole file could be empty or contain valid node
file
	: node    { std::cout << "file (node)\n";  builder.root = $1; }
	| %empty  { builder.root = Node_internal(NODETYPE_NONE); }
	;
	

node
	: node_2 { std::cout << "node\n"; $$ = $1; }
	| node '+' node_2 { $$ = $1; }
	;

node_2
	: lscl_map { $$ = $1; }
	| lscl_list { std::cout << "node_2: list\n"; $$ = $1; }
	| scalar { std::cout << "node_2: scalar\n"; $$ = $1; }
	;

lscl_list: '[' lscl_list_body ']' { std::cout << "lscl_list (size = " << $2->size() << ")\n"; $$ = Node_internal($2); } ;

lscl_list_body
	: %empty { std::cout << "lscl_list_body: empty\n"; $$ = std::make_shared<Node_internal::lscl_list>(); }
	| node   {
		std::cout << "lscl_list_body: single node\n";
		auto temp_ptr = std::make_shared<Node_internal::lscl_list>();
		temp_ptr->push_back($1);
		$$ = temp_ptr;
	}
	| lscl_list_body ',' node  {
		std::cout << "lscl_list_body: comma-repeated\n";
		$1->push_back($3);
		$$ = $1;
	}
	| lscl_list_body '\n' node {
		std::cout << "lscl_list_body: newline-repeated\n";
		$1->push_back($3);
		$$ = $1;
	}
	;

lscl_map: '{' lscl_map_body '}' { std::cout << "lscl_map (size = " << $2->size() << ")\n"; $$ = Node_internal($2); } ;

lscl_map_body
	: %empty {
		std::cout << "lscl_map_body: empty\n";
		$$ = std::make_shared<Node_internal::lscl_map>();
	}
	| scalar ':' node {
		std::cout << "lscl_map_body: single node\n";
		auto temp_ptr = std::make_shared<Node_internal::lscl_map>();
		temp_ptr->insert(
			{
				$1.value, // key
				$3        // value
			}
		);
		$$ = temp_ptr;
	}
	| lscl_map_body ',' scalar ':' node {
		std::cout << "lscl_map_body: comma-repeated\n";
		$1->insert(
			{
				$3.value, // key
				$5        // value
			}
		);
		$$ = $1;
	}
	| lscl_map_body '\n' scalar ':' node {
		std::cout << "lscl_map_body: newline-repeated\n";
		$1->insert(
			{
				$3.value, // key
				$5        // value
			}
		);
		$$ = $1;
	}
	;

scalar
	: SCALAR_PLAINTEXT {
		$$ = Node_internal(
			process_scalar_plaintext($1) // Preserve nothing
		);
	}
	| SCALAR_SINGLE_Q {
		$$ = Node_internal(
			process_scalar_quotes_single($1, 0) // Preserve nothing
		);
	}
	| '<' SCALAR_SINGLE_Q '>' {
		$$ = Node_internal(
			process_scalar_quotes_single($2, 1) // Preserve newlines
		);
	}
	| '<' '<' SCALAR_SINGLE_Q '>' '>' {
		$$ = Node_internal(
			process_scalar_quotes_single($3, 2) // Preserve everything
		);
	}
	| SCALAR_DOUBLE_Q {
		$$ = Node_internal(
			process_scalar_quotes_double($1, 0) // Preserve nothing
		);
	}
	| '<' SCALAR_DOUBLE_Q '>' {
		$$ = Node_internal(
			process_scalar_quotes_double($2, 1) // Preserve newlines
		);
	}
	| '<' '<' SCALAR_DOUBLE_Q '>' '>' {
		$$ = Node_internal(
			process_scalar_quotes_double($3, 2) // Preserve everything
		);
	}
	;

%%


void LSCL::Nodebuilder::LSCL_Parser::error(const location_type &l, const std::string &err_message)
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
