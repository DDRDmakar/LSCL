%{
// C++ string header, for string ops below
#include <string>

// Implementation of yyFlexScanner
#include "lscl_scanner.hpp"
#undef  YY_DECL
#define YY_DECL int LSCL::Nodebuilder::LSCL_Scanner::yylex(LSCL::Nodebuilder::LSCL_Parser::semantic_type* const lval, LSCL::Nodebuilder::LSCL_Parser::location_type *loc)

// typedef to make the returns for the tokens shorter
using token = LSCL::Nodebuilder::LSCL_Parser::token;

// define yyterminate as this instead of NULL
#define yyterminate() return( token::END )

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

%%
%{
    // Code executed at the beginning of yylex
    yylval = lval;
%}

.           {
               return yytext[0];
            }
%%


