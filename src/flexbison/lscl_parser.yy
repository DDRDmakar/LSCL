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
	
	#define INSERT_SCALAR_INTO_LIST(nod)                                       \
		if (nod.type == NODETYPE_SCALAR)                                       \
		{                                                                      \
			builder.workpoint->values_list.push_back(nod);                     \
			builder.workpoint->values_list.back().parent = builder.workpoint;  \
		}                                                                      \
	
	
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

%type  <LSCL::Node_internal>      scalar
%type  <LSCL::Node_internal>      node
%type  <LSCL::Node_internal>      node_2

%locations

%%

// Whole file could be empty or contain valid node
file
	: node    { std::cout << "file (node)\n"; }
	| %empty  {
		
		std::cout << "file (empty)\n";
		builder.root_created = true;
		builder.root = std::make_shared<Node_internal>(NODETYPE_NONE, nullptr); // NONE node on the tree root
		builder.workpoint = builder.root.get();
	}
	;
	

node
	: node_2 { std::cout << "node\n"; $$ = $1; }
	| node '+' node_2
	;

node_2
	: lscl_map { $$ = Node_internal(NODETYPE_NONE); }
	| lscl_list { std::cout << "node_2: list\n"; $$ = Node_internal(NODETYPE_NONE); }
	| scalar {
		std::cout << "node_2: scalar\n";
		// If file contains only one scalar
		if (!builder.root_created)
		{
			builder.root_created = true;
			std::cout << "File contains scalar - creating it" << std::endl;
			builder.root = std::make_shared<LSCL::Node_internal>($1); // Create scalar at tree root
			builder.workpoint = builder.root.get();
			$$ = Node_internal(NODETYPE_NONE);
		}
		$$ = $1;
	}
	;

open_square: '[' {
	
	if (!builder.root_created)
	{
		builder.root_created = true;
		std::cout << "Beginning of list - creating list at tree root" << std::endl;
		builder.root = std::make_shared<LSCL::Node_internal>(LSCL::NODETYPE_LIST, nullptr); // Create list at tree root
		builder.workpoint = builder.root.get();
	}
	else
	{
		
	}
	builder.nodestack.push(NODEWAY_LIST);
};

lscl_list: open_square lscl_list_body ']' { std::cout << "lscl_list\n"; } ;

lscl_list_body
	: %empty { std::cout << "lscl_list_body: empty\n"; }
	| node   {
		std::cout << "lscl_list_body: single node\n";
		INSERT_SCALAR_INTO_LIST($1);
	}
	| lscl_list_body ',' node  { std::cout << "lscl_list_body: comma-repeated\n"; }
	| lscl_list_body '\n' node { std::cout << "lscl_list_body: newline-repeated\n"; }
	;

open_curved: '{' {
	
	if (!builder.root_created)
	{
		builder.root_created = true;
		std::cout << "Beginning of map - creating map at tree root" << std::endl;
		builder.root = std::make_shared<LSCL::Node_internal>(NODETYPE_MAP, nullptr); // Create map at tree root
		builder.workpoint = builder.root.get();
	}
	builder.nodestack.push(NODEWAY_LIST);
};


lscl_map: open_curved lscl_map_body '}' ;

kv_pair: scalar ':' node ;

lscl_map_body
	: %empty
	| kv_pair
	| lscl_map_body ',' kv_pair
	| lscl_map_body '\n' kv_pair
	;

scalar
	: SCALAR_PLAINTEXT {
		$$ = LSCL::Node_internal(
			nullptr,
			process_scalar_plaintext($1, false)
		);
	}
	| '<' SCALAR_PLAINTEXT '>' {
		$$ = LSCL::Node_internal(
			nullptr,
			process_scalar_plaintext($2, true)
		);
	}
	| SCALAR_SINGLE_Q {
		$$ = Node_internal(
			nullptr,
			process_scalar_quotes_single($1, false)
		);
	}
	| '<' SCALAR_SINGLE_Q '>' {
		$$ = Node_internal(
			nullptr,
			process_scalar_quotes_single($2, true)
		);
	}
	| SCALAR_DOUBLE_Q {
		$$ = Node_internal(
			nullptr,
			process_scalar_quotes_double($1, false)
		);
	}
	| '<' SCALAR_DOUBLE_Q '>' {
		$$ = Node_internal(
			nullptr,
			process_scalar_quotes_double($2, true)
		);
	}
	;

%%


void LSCL::Nodebuilder::LSCL_Parser::error(const location_type &l, const std::string &err_message)
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
