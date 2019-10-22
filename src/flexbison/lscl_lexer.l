%{
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



// C++ string header, for string ops below
#include <string>

// Implementation of yyFlexScanner
#include "lscl_scanner.hpp"
#undef  YY_DECL
#define YY_DECL                                                     \
	int LSCL::Nodebuilder::LSCL_Scanner::yylex(                     \
		LSCL::Nodebuilder::LSCL_Parser::semantic_type* const lval,  \
		LSCL::Nodebuilder::LSCL_Parser::location_type *loc          \
	)                                                               \


// typedef to make the returns for the tokens shorter
using token = LSCL::Nodebuilder::LSCL_Parser::token;

// define yyterminate as this instead of NULL
//#define yyterminate() return( token::END_OF_FILE )

// msvc2010 requires that we exclude this header file.
#define YY_NO_UNISTD_H

// update location on matching
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="LSCL::Nodebuilder::LSCL_Scanner"
%option noyywrap
%option c++

link_unquoted     [^ '\",{\}\[\]\<\>\:/\+\n&\t\(\)@]+

%%
%{
    // Code executed at the beginning of yylex
    yylval = lval;
%}

<<EOF>>  { return '\0'; }



\"((([^\\\"])|\n|(\\[^\"\\])|((\\\\)*(\\\")?))*)\" {
	
	std::cout << "double q: |" << yytext << '|' << std::endl;
	yylval->build<std::string>(yytext);
	return token::SCALAR_DOUBLE_Q;
}

'((([^'])|\n|(''))*)' {
	
	std::cout << "single q: |" << yytext << '|' << std::endl;
	yylval->build<std::string>(yytext);
	return token::SCALAR_SINGLE_Q;
}

[^ '\",{\}\[\]\<\>\:/\+\n&\*\t@]([^'\",{\}\[\]\<\>\:/\+\n@]|\/[^\/])* {
	
	std::cout << "plaintext: |" << yytext << '|' << std::endl;
	yylval->build<std::string>(yytext);
	return token::SCALAR_PLAINTEXT;
}

&{link_unquoted} {
	
	std::cout << "link creation: |" << yytext << '|' << std::endl;
	yylval->build<std::string>(yytext);
	return token::LINK_SET;
}

\*{link_unquoted} {
	
	std::cout << "link usage: |" << yytext << '|' << std::endl;
	yylval->build<std::string>(yytext);
	return token::LINK_USE;
}

@{link_unquoted} {
	
	std::cout << "linked node copying: |" << yytext << '|' << std::endl;
	yylval->build<std::string>(yytext);
	return token::LINK_COPY;
}

(\t|" "|\n)+ ; // Skip spacers

. {
	std::cout << "char: " << yytext << std::endl;
	return yytext[0];
}

%%


