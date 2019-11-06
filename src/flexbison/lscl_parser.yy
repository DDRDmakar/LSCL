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
	#include <sstream>
	
	// include for all builder functions
	#include "nodebuilder.hpp"
	
	#undef  yylex
	#define yylex scanner.yylex
	
}

%define parse.assert
%define api.value.type variant

%token <std::string>         SCALAR_PLAINTEXT
%token <std::string>         SCALAR_DOUBLE_Q
%token <std::string>         SCALAR_SINGLE_Q
%token <std::string>         SCALAR_ACUTE_Q
%token <std::string>         LINK_SET
%token <std::string>         LINK_USE
%token <std::string>         LINK_COPY

%type  <LSCL::Scalar*>                             scalar
%type  <LSCL::Scalar*>                             scalar_quoted
%type  <Node_internal*>                            node
%type  <Node_internal*>                            node_1
%type  <Node_internal*>                            node_2
%type  <LSCL::Map::lscl_map*>                      lscl_map_body
%type  <Node_internal*>                            lscl_map
%type  <LSCL::List::lscl_list*>                    lscl_list_body
%type  <Node_internal*>                            lscl_list

%type  <std::string>                  link_set
%type  <LSCL::Link*>                  link_use

%type <Link::lscl_path*>              reference
%type <Link::lscl_path*>              reference_body
%type <Link::lscl_path_element>       reference_newelement

%type  <Node_internal*>               script_use

%locations

%%

// Whole file could be empty or contain valid node
file
	: node    { std::cout << "file (node)\n"; builder.setroot($1); }
	| %empty  { std::cout << "file (empty)\n"; builder.setroot(new Node_empty()); }
	;

node
	: node_1 { $$ = $1; }
	| link_set node_1 {
		builder.set_link($1, $2);
		$$ = $2;
	}
	;

node_1
	: node_2 { $$ = $1; }
	| node_1 '+' node_2 { // Sum two nodes
		$$ = $1;
	}
	;

node_2
	: lscl_map { $$ = $1; }
	| lscl_list { std::cout << "node_2: list\n"; $$ = $1; }
	| scalar { std::cout << "node_2: scalar\n"; $$ = $1; }
	| link_use {
		//std::cout << "node_2: link: " << $1 << std::endl;
		//builder.use_link(name, $1);
		$$ = $1;
	}
	| script_use { $$ = $1; }
	;

lscl_list
	: '[' lscl_list_body ']'     {
		std::cout << "lscl_list (size = " << $2->size() << ")\n";
		$$ = new List($2);
	}
	| '[' lscl_list_body ',' ']' {
		std::cout << "lscl_list (size = " << $2->size() << ")\n";
		$$ = new List($2);
	}
	;

lscl_list_body
	: %empty { std::cout << "lscl_list_body: empty\n"; $$ = new List::lscl_list(); }
	| node   {
		std::cout << "lscl_list_body: single node\n";
		auto temp_ptr = new List::lscl_list();
		temp_ptr->push_back($1);
		$$ = temp_ptr;
	}
	| lscl_list_body ',' node  {
		std::cout << "lscl_list_body: comma-repeated\n";
		$1->push_back($3);
		$$ = $1;
	}
	;

lscl_map
	: '{' lscl_map_body '}'     {
		std::cout << "lscl_map (size = " << $2->size() << ")\n";
		$$ = new Map($2);
	}
	| '{' lscl_map_body ',' '}' {
		std::cout << "lscl_map (size = " << $2->size() << ")\n";
		$$ = new Map($2);
	}
	;

lscl_map_body
	: %empty {
		std::cout << "lscl_map_body: empty\n";
		$$ = new Map::lscl_map();
	}
	| scalar ':' node {
		std::cout << "lscl_map_body: single node\n";
		auto temp_ptr = new Map::lscl_map();
		auto inserted = temp_ptr->insert(
			{
				$1->value, // key
				$3         // value
			}
		);
		if (!inserted.second) throw LSCL::Exception::Exception_nodebuilder("Element with key \"" + $1->value + "\" not inserted into map", builder.get_filename());
		$$ = temp_ptr;
	}
	| lscl_map_body ',' scalar ':' node {
		std::cout << "lscl_map_body: comma-repeated\n";
		auto inserted = $1->insert(
			{
				$3->value, // key
				$5         // value
			}
		);
		if (!inserted.second) throw LSCL::Exception::Exception_nodebuilder("Element with key \"" + $3->value + "\" not inserted into map", builder.get_filename());
		$$ = $1;
	}
	;

scalar
	: SCALAR_PLAINTEXT {
		$$ = new Scalar(
			builder.process_scalar_plaintext($1),
			$1
		);
	}
	| scalar_quoted { $$ = $1; }
	;

scalar_quoted
	: SCALAR_SINGLE_Q {
		$$ = new Scalar(
			builder.process_scalar_quotes_single($1, 0), // Preserve nothing
			$1
		);
	}
	| '<' SCALAR_SINGLE_Q '>' {
		$$ = new Scalar(
			builder.process_scalar_quotes_single($2, 1), // Preserve newlines
			$2
		);
	}
	| '<' '<' SCALAR_SINGLE_Q '>' '>' {
		$$ = new Scalar(
			builder.process_scalar_quotes_single($3, 2), // Preserve everything
			$3
		);
	}
	| SCALAR_DOUBLE_Q {
		$$ = new Scalar(
			builder.process_scalar_quotes_double($1, 0), // Preserve nothing
			$1
		);
	}
	| '<' SCALAR_DOUBLE_Q '>' {
		$$ = new Scalar(
			builder.process_scalar_quotes_double($2, 1), // Preserve newlines
			$2
		);
	}
	| '<' '<' SCALAR_DOUBLE_Q '>' '>' {
		$$ = new Scalar(
			builder.process_scalar_quotes_double($3, 2), // Preserve everything
			$3
		);
	}
	;

link_set
	: LINK_SET {
		std::string ts = $1;  // Temporary string
		ts.erase(ts.begin()); // Remove first symbol
		$$ = ts;              // Return as name
	}
	| '&' scalar_quoted {
		$$ = $2->value;        // Just return $2 text
	}
	;

// Link to node
link_use
	: LINK_USE {
		std::string ts = $1;  // Temporary string
		ts.erase(ts.begin()); // Remove first symbol
		Link *lp = new Link(ts, false);
		builder.use_link(ts, lp);
		$$ = lp;
	}
	| '*' scalar_quoted {
		Link *lp = new Link($2->value, false);
		builder.use_link($2->value, lp);
		$$ = lp;
	}
	| '*' reference {
		Link *lp = new Link($2, false);
		builder.use_ref(lp);
		$$ = lp;
	}
	// Create a copy of linked node
	| LINK_COPY {
		std::string ts = $1;  // Temporary string
		ts.erase(ts.begin()); // Remove first symbol
		Link *lp = new Link(ts, true);
		builder.use_link(ts, lp);
		$$ = lp;
	}
	| '@' scalar_quoted {
		Link *lp = new Link($2->value, true);
		builder.use_link($2->value, lp);
		$$ = lp;
	}
	| '@' reference {
		Link *lp = new Link($2, true);
		builder.use_ref(lp);
		$$ = lp;
	}
	;

// Brackets with path to node inside it
reference
	: '(' reference_body ')' {
		std::cout << "REFERENCE ";
		for (const auto &e : *$2) std::cout << e.text << '|' << e.idx << '|' << e.is_idx << "    ";
		std::cout << std::endl;
		$$ = $2;
	}
	;

reference_newelement
	: SCALAR_PLAINTEXT {
		std::cout << "Plaintext reference element" << std::endl;
		$$ = {$1, 0, false};
	}
	| scalar_quoted {
		std::cout << "Quoted reference element" << std::endl;
		$$ = {$1->value, 0, false};
	}
	| '['  SCALAR_PLAINTEXT ']' {
		std::cout << "Index reference element" << std::endl;
		bool space = false;
		for (const char c : $2)
		{
			// Here we check if there are forbidden symbols in index string.
			// Spaces in the begin and end are available
			if (is_spacer(c)) { space = true; continue; }
			if (space || '0' > c || c > '9') throw Exception::Exception_nodebuilder("Unable to convert [" + $2 + "] into index in reference", builder.get_filename());
		}
		size_t idx = std::stoul($2);
		$$ = {"", idx, true};
	}
	;

reference_body
	: reference_newelement {
		std::cout << "Reference first element" << std::endl;
		Link::lscl_path *temp_ptr = new Link::lscl_path;
		temp_ptr->push_back($1);
		$$ = temp_ptr;
	}
	| reference_body '.' reference_newelement {
		std::cout << "Reference next element" << std::endl;
		$1->push_back($3);
		$$ = $1;
	}
	;

/*node_inherited
	: '~' reference {
	}*/
	
script_use
	: SCALAR_ACUTE_Q { // Here we execute script in the separate thread
		
		Builder::Executed &executed = builder.add_executed();
		Attached *target = new Attached();
		
		executed.type = Builder::EXECUTED_TYPE_SCRIPT;
		//executed.thr = 
		executed.target = target;
		executed.args = {
			builder.process_acute_text($1),
			nullptr,
			false,
			nullptr
		};
		if (
			pthread_create(
				&executed.thr,
				NULL,
				script_processor,
				&executed.args
			)
		)
		{
			throw Exception::Exception_nodebuilder("Unable to create script-processing thread", builder.get_filename());
		}
		
		$$ = target;
	}
	;


%%


void LSCL::Nodebuilder::LSCL_Parser::error(const location_type &l, const std::string &err_message)
{
	std::stringstream ss;
	ss << err_message << " at " << l;
	throw LSCL::Exception::Exception_nodebuilder(ss.str(), builder.get_filename());
}
