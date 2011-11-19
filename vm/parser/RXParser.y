
%{
#include <stdio.h>
#include <string.h>
#include <Eina.h>
#include <Reflexio.h>

int yyerror(const char* fmt) {
   fprintf(stderr, "%s\n", fmt);
   return 0;
}

int yylex();

static Eina_Array* RXParser_stack;
static int RXParser_stackTop;

void RXParser_init() {
   RXParser_stackTop = -1;
   RXParser_stack = eina_array_new(4);
}

void RXParser_destroy() {
   eina_array_free(RXParser_stack);
}

void RXParser_push(void* object) {
   eina_array_push(RXParser_stack, object);
   ++ RXParser_stackTop;
}

void* RXParser_pop() {
   -- RXParser_stackTop;
   return eina_array_pop(RXParser_stack);
}

void RXParser_appendMessage(RXObject_t* message) {
   RXObject_t* expression = eina_array_data_get(RXParser_stack, RXParser_stackTop);
   RXList_append(expression, message);
}

void RXParser_appendArgument(RXObject_t* expression) {
   RXObject_t* message = eina_array_data_get(RXParser_stack, RXParser_stackTop);
   RXList_append(message, expression);
}

RXObject_t* RXParser_messageWithName(RXObject_t* name) {
    RXObject_t* result = RXList_spawn(RXMessage_o);
    RXObject_setSlot(result, RXSymbol_name_o, name);
    return result;
}

%}

%error-verbose

%union {
   int asInteger;
   double asReal;
   char* asString;
   char* asIdentifier;
   void* asObject;
}

%token LPAR
%token RPAR
%token COLON
%token SEMI
%token COMMA
%token PLUS
%token MINUS
%token STAR
%token SLASH
%token ASSIGN
%token EQ
%token NEQ
%token LT
%token GT
%token LEQ
%token GEQ
%token OR
%token AND

%token <asString> STRING
%token <asInteger> INTEGER
%token <asReal> REAL
%token <asIdentifier> IDENTIFIER

%type <asObject> boolean_operator
%type <asObject> comparison_operator
%type <asObject> additive_operator
%type <asObject> multiplicative_operator
%type <asObject> unary_operator

%%

expression:
   optional_expression_separator non_empty_expression optional_expression_separator
   ;

expression_separator:
   SEMI
   | expression_separator SEMI
   ;

optional_expression_separator:
   /* Empty */
   | expression_separator
   ;

non_empty_expression:
   boolean_expression
   | non_empty_expression expression_separator {
         RXParser_appendMessage(RXParser_messageWithName(RXSymbol_semicolon_o));
      } boolean_expression
   ;

boolean_expression:
   comparison_expression
   | boolean_expression boolean_operator {
         RXParser_push($2);
         RXParser_push(RXList_spawn(RXExpression_o));
      } comparison_expression {
         RXParser_appendArgument(RXParser_pop());
         RXParser_appendMessage(RXParser_pop());
      }
   ;

boolean_operator:
   OR { $$ = RXParser_messageWithName(RXSymbol_or_o); }
   | AND { $$ = RXParser_messageWithName(RXSymbol_and_o); }
   ;

comparison_expression:
   additive_expression
   | comparison_expression comparison_operator {
         RXParser_push($2);
         RXParser_push(RXList_spawn(RXExpression_o));
      } additive_expression {
         RXParser_appendArgument(RXParser_pop());
         RXParser_appendMessage(RXParser_pop());
      }
   ;

comparison_operator:
   EQ { $$ = RXParser_messageWithName(RXSymbol_equal_o); }
   | NEQ { $$ = RXParser_messageWithName(RXSymbol_notEqual_o); }
   | LT { $$ = RXParser_messageWithName(RXSymbol_lessThan_o); }
   | GT { $$ = RXParser_messageWithName(RXSymbol_greaterThan_o); }
   | LEQ { $$ = RXParser_messageWithName(RXSymbol_lessOrEqual_o); }
   | GEQ { $$ = RXParser_messageWithName(RXSymbol_greaterOrEqual_o); }
   ;

additive_expression:
   multiplicative_expression
   | additive_expression additive_operator {
         RXParser_push($2);
         RXParser_push(RXList_spawn(RXExpression_o));
      } multiplicative_expression {
         RXParser_appendArgument(RXParser_pop());
         RXParser_appendMessage(RXParser_pop());
      }
   ;

additive_operator:
   PLUS { $$ = RXParser_messageWithName(RXSymbol_add_o); }
   | MINUS { $$ = RXParser_messageWithName(RXSymbol_subtract_o); }
   ;

multiplicative_expression:
   unary_expression
   | multiplicative_expression multiplicative_operator {
         RXParser_push($2);
         RXParser_push(RXList_spawn(RXExpression_o));
      } unary_expression {
         RXParser_appendArgument(RXParser_pop());
         RXParser_appendMessage(RXParser_pop());
      }
   ;

multiplicative_operator:
   STAR { $$ = RXParser_messageWithName(RXSymbol_multiply_o); }
   | SLASH { $$ = RXParser_messageWithName(RXSymbol_divide_o); }
   ;

unary_expression:
   simple_expression
   | unary_operator simple_expression {
         RXParser_appendMessage($1); 
      }
   ;

unary_operator:
   PLUS { $$ = RXParser_messageWithName(RXSymbol_unaryPlus_o); }
   | MINUS { $$ = RXParser_messageWithName(RXSymbol_negate_o); }
   ;

simple_expression:
   first_receiver message_chain
   ;

first_receiver:
   method_call_or_assignment
   | STRING { RXParser_appendMessage(RXSymbol_symbolForCString($1)); }
   | INTEGER { RXParser_appendMessage(RXInteger_spawn(RXInteger_o, $1)); }
/* TODO Add support for real numbers
   | REAL { RXParser_appendMessage(RXReal_fromCReal($1)); }
*/
   | LPAR {
         RXParser_push(RXList_spawn(RXExpression_o));
      } non_empty_expression RPAR {
         RXParser_appendMessage(RXParser_pop());
      }
   ;

message_chain:
   /* Empty */
   | message_chain method_call_or_assignment
   ;

method_call_or_assignment:
   IDENTIFIER { RXParser_appendMessage(RXParser_messageWithName(RXSymbol_symbolForCString($1))); }
   | IDENTIFIER LPAR RPAR { RXParser_appendMessage(RXParser_messageWithName(RXSymbol_symbolForCString($1))); }
   | IDENTIFIER LPAR {
         RXParser_push(RXParser_messageWithName(RXSymbol_symbolForCString($1)));
         RXParser_push(RXList_spawn(RXExpression_o));
      } argument_list RPAR {
         RXParser_appendMessage(RXParser_pop());
      }
   | IDENTIFIER ASSIGN {
         RXParser_push(RXParser_messageWithName(RXSymbol_setSlot_o));
         RXParser_push(RXList_spawn(RXExpression_o));
         RXParser_appendMessage(RXSymbol_symbolForCString($1));
         RXParser_appendArgument(RXParser_pop());
         RXParser_push(RXList_spawn(RXExpression_o));
      }
      comparison_expression {
         RXParser_appendArgument(RXParser_pop());
         RXParser_appendMessage(RXParser_pop());
      }
   ;

argument_list:
   argument
   | argument_list COMMA {
         RXParser_push(RXList_spawn(RXExpression_o));
      } argument
   ;

argument:
   non_empty_expression {
         RXObject_t* argumentExpression = RXParser_pop();
         RXParser_appendArgument(argumentExpression);
      }
   ;

%%

#if 0
extern FILE* yyin;

int main(int argc, char* argv[]) {
   ++argv, --argc;
   if ( argc > 0 )
      yyin = fopen( argv[0], "r" );
   else
      yyin = stdin;
   yyparse();
}
#else

extern FILE* yyin;
void yy_scan_string(const char* str);

RXObject_t* RXParser_parse() {
   RXParser_init();
   RXParser_push(RXList_spawn(RXExpression_o));
   yyparse();
   RXObject_t* result = RXParser_pop();
   RXParser_destroy();
   return result;
}

RXObject_t* RXParser_expressionFromCString(char* string) {
   yy_scan_string(string);
   return RXParser_parse();
}

RXObject_t* RXParser_expressionFromCFile(FILE* file) {
   yyin = file;
   return RXParser_parse();
}

#endif

