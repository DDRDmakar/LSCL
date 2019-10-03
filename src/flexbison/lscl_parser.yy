%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {LSCL::Nodebuilder}
%define parser_class_name {LSCL_Parser}

%code requires {
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

%define api.value.type variant
%define parse.assert

%token               END
%token <std::string> PLAINTEXT
%token               NEWLINE
%token               SPACER

%locations

%%

node: lscl_map | lscl_list | scalar ;

lscl_list: '[' lscl_list_body ']' ;

lscl_list_body
	: node
	| lscl_list_body ',' node
	| lscl_list_body '\n' node
	;

lscl_map: '{' lscl_map_body '}' ;

kv_pair: scalar ':' node ;

lscl_map_body
	: kv_pair
	| lscl_map_body ',' kv_pair
	| lscl_map_body '\n' kv_pair
	;

scalar: _scalar | _scalar_angle ;

_scalar_angle: '<' _scalar '>' ;

_scalar
	: _scalar_quote_single
	| _scalar_quote_double
	| _scalar_unquoted
	;

_scalar_quote_single: 'a' ;
_scalar_quote_double: 'a' ;
_scalar_unquoted:     'a' ;

/*
item
  : UPPER   { builder.add_upper(); }
  | LOWER   { builder.add_lower(); }
  | WORD    { builder.add_word( $1 ); }
  | NEWLINE { builder.add_newline(); }
  | CHAR    { builder.add_char(); }
  ;
*/

%%


void LSCL::Nodebuilder::LSCL_Parser::error(const location_type &l, const std::string &err_message)
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
