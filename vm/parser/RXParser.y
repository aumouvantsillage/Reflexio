
%{
#include <stdio.h>
#include <string.h>
#include <Eina.h>
#include <Reflexio.h>

int yyerror(const char* msg) {
   fprintf(stderr, "%s\n", msg);
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
    RXObject_t* result = RXList_spawn(RXMessage_o, NULL);
    RXObject_setSlot(result, RXSymbol_name_o, name);
    return result;
}

void RXParser_pushOperator(RXObject_t* name) {
    RXParser_push(RXParser_messageWithName(name));
    RXParser_push(RXList_spawn(RXExpression_o, NULL));
}

void RXParser_completeBinaryExpression(void) {
     RXParser_appendArgument(RXParser_pop());
     RXParser_appendMessage(RXParser_pop());
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

%left OR
%left AND
%nonassoc EQ NEQ LT GT LEQ GEQ
%left PLUS MINUS
%left STAR SLASH
%left UNARY

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
   operator_expression
   | non_empty_expression expression_separator {
         RXParser_appendMessage(RXParser_messageWithName(RXSymbol_semicolon_o));
      } operator_expression
   ;

operator_expression:
   simple_expression
   | operator_expression OR    {RXParser_pushOperator(RXSymbol_or_o);}             operator_expression {RXParser_completeBinaryExpression();}
   | operator_expression AND   {RXParser_pushOperator(RXSymbol_and_o);}            operator_expression {RXParser_completeBinaryExpression();}
   | operator_expression EQ    {RXParser_pushOperator(RXSymbol_equal_o);}          operator_expression {RXParser_completeBinaryExpression();}
   | operator_expression NEQ   {RXParser_pushOperator(RXSymbol_notEqual_o);}       operator_expression {RXParser_completeBinaryExpression();}
   | operator_expression LT    {RXParser_pushOperator(RXSymbol_lessThan_o);}       operator_expression {RXParser_completeBinaryExpression();}
   | operator_expression GT    {RXParser_pushOperator(RXSymbol_greaterThan_o);}    operator_expression {RXParser_completeBinaryExpression();}
   | operator_expression LEQ   {RXParser_pushOperator(RXSymbol_lessOrEqual_o);}    operator_expression {RXParser_completeBinaryExpression();}
   | operator_expression GEQ   {RXParser_pushOperator(RXSymbol_greaterOrEqual_o);} operator_expression {RXParser_completeBinaryExpression();}
   | operator_expression PLUS  {RXParser_pushOperator(RXSymbol_add_o);}            operator_expression {RXParser_completeBinaryExpression();}
   | operator_expression MINUS {RXParser_pushOperator(RXSymbol_subtract_o);}       operator_expression {RXParser_completeBinaryExpression();}
   | operator_expression STAR  {RXParser_pushOperator(RXSymbol_multiply_o);}       operator_expression {RXParser_completeBinaryExpression();}
   | operator_expression SLASH {RXParser_pushOperator(RXSymbol_divide_o);}         operator_expression {RXParser_completeBinaryExpression();}
   |                     PLUS  operator_expression %prec UNARY {RXParser_appendMessage(RXParser_messageWithName(RXSymbol_unaryPlus_o));}
   |                     MINUS operator_expression %prec UNARY {RXParser_appendMessage(RXParser_messageWithName(RXSymbol_negate_o));}
   ;

simple_expression:
   first_receiver message_chain
   ;

first_receiver:
   message_or_assignment
   | STRING { RXParser_appendMessage(RXSymbol_symbolForCString($1)); }
   | INTEGER { RXParser_appendMessage(RXInteger_spawn(RXInteger_o, $1)); }
/* TODO Add support for real numbers
   | REAL { RXParser_appendMessage(RXReal_fromCReal($1)); }
*/
   | LPAR {
         RXParser_push(RXList_spawn(RXExpression_o, NULL));
      } non_empty_expression RPAR {
         RXParser_appendMessage(RXParser_pop());
      }
   ;

message_chain:
   /* Empty */
   | message_chain message_or_assignment
   ;

message_or_assignment:
   IDENTIFIER { RXParser_appendMessage(RXParser_messageWithName(RXSymbol_symbolForCString($1))); }
   | IDENTIFIER LPAR RPAR { RXParser_appendMessage(RXParser_messageWithName(RXSymbol_symbolForCString($1))); }
   | IDENTIFIER LPAR {
         RXParser_push(RXParser_messageWithName(RXSymbol_symbolForCString($1)));
         RXParser_push(RXList_spawn(RXExpression_o, NULL));
      } argument_list RPAR {
         RXParser_appendMessage(RXParser_pop());
      }
   | IDENTIFIER ASSIGN {
         RXParser_push(RXParser_messageWithName(RXSymbol_setSlot_o));
         RXParser_push(RXList_spawn(RXExpression_o, NULL));
         RXParser_appendMessage(RXSymbol_symbolForCString($1));
         RXParser_appendArgument(RXParser_pop());
         RXParser_push(RXList_spawn(RXExpression_o, NULL));
      }
      operator_expression {
         RXParser_appendArgument(RXParser_pop());
         RXParser_appendMessage(RXParser_pop());
      }
   ;

argument_list:
   argument
   | argument_list COMMA {
         RXParser_push(RXList_spawn(RXExpression_o, NULL));
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
   RXParser_push(RXList_spawn(RXExpression_o, NULL));
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

