
%{
#include "RXTokens.h"

char* unescape(char* str) {
   // Count string length, removing quotes and backslashes
   int len = -2;
   for(char* ptr = str; *ptr; ptr++) {
      if(*ptr != '\\' || *(ptr+1) == 0)
         len++;
   }

   char* result = malloc((len+1)*sizeof(char));
   char* presult = result;
   char* ptr = str+1;
   while(len) {
      if(*ptr != '\\' || *(ptr+1) == 0)
         *presult++ = *ptr;
      else {
         ptr++;
         switch(*ptr) {
            case 'r': *presult++ = '\r'; break;
            case 'n': *presult++ = '\n'; break;
            case 't': *presult++ = '\t'; break;
            case '"': *presult++ = '\"'; break;
            default: *presult++ = *ptr;
         }
      }
      ++ ptr;
      -- len;
   }
   *presult = 0;
   return result;
}

%}

%option noyywrap

DIGIT [0-9]
HEXDIGIT [0-9a-fA-F]
ALPHA [a-zA-Z]
NL [\r\n]
WS [ \t]
LC "//".*
MC "/*"(.|{NL})*"*/"
SKIP ({LC}|{MC}|{NL}|{WS})*
%%

"or" { return OR; }
"and" { return AND; }

"("{SKIP} { return LPAR; }
")" { return RPAR; }
":"{SKIP} { return COLON; }
";"{SKIP} { return SEMI; }
","{SKIP} { return COMMA; }
"+"{SKIP} { return PLUS; }
"-"{SKIP} { return MINUS; }
"*"{SKIP} { return STAR; }
"/"{SKIP} { return SLASH; }
"="{SKIP} { return ASSIGN; }
"=="{SKIP} { return EQ; }
"!="{SKIP} { return NEQ; }
"<"{SKIP} { return LT; }
">"{SKIP} { return GT; }
"<="{SKIP} { return LEQ; }
">="{SKIP} { return GEQ; }

"~"{SKIP}
{SKIP}/")"

({WS}|{LC}|{MC})*{NL}{SKIP} { return SEMI; }
{SKIP}

({ALPHA}|"_"|"$")({ALPHA}|{DIGIT}|"_")* {
   yylval.asIdentifier = strdup(yytext);
   return IDENTIFIER;
}

{DIGIT}+ {
   yylval.asInteger = strtol(yytext, 0, 10);
   return INTEGER;
}

"0x"{HEXDIGIT}+ {
   yylval.asInteger = strtol(yytext, 0, 16);
   return INTEGER;
}

{DIGIT}+"."{DIGIT}+ {
   yylval.asReal = strtod(yytext, 0);
   return REAL;
}

"\""([^"]|"\\\"")*"\"" {
   int len = strlen(yytext)-2;
   yylval.asString = unescape(yytext);
   return STRING;
}

%%

#if 0
int main(int argc, char* argv[]) {
   ++argv, --argc;
   if ( argc > 0 )
      yyin = fopen( argv[0], "r" );
   else
      yyin = stdin;
   while(yylex());
}
#endif

