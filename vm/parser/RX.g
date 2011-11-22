
grammar RX;

options {
	language = C;
	backtrack=true;
}

@header {
	#include <core/RXCore.h>
	#include <stdio.h>
	
	RXObject_t* RXParser_expressionFromCString(char* string);
	RXObject_t* RXParser_expressionFromCFile(char* fileName);
}

@members {
	#include <Eina.h>
	#include <lib/RXLib.h>
	#include <antlr3.h>
	#include "RXLexer.h"
	
	static Eina_Array* RXParser_stack;
	static int RXParser_stackTop;

	static void RXParser_setup() {
	   RXParser_stackTop = -1;
	   RXParser_stack = eina_array_new(4);
	}

	static void RXParser_clean() {
	   eina_array_free(RXParser_stack);
	}

	static void RXParser_push(void* object) {
	   eina_array_push(RXParser_stack, object);
	   ++ RXParser_stackTop;
	}

	static void* RXParser_pop() {
	   -- RXParser_stackTop;
	   return eina_array_pop(RXParser_stack);
	}

	static void RXParser_appendMessage(RXObject_t* message) {
	   RXObject_t* expression = eina_array_data_get(RXParser_stack, RXParser_stackTop);
	   RXList_append(expression, message);
	}

	static void RXParser_appendArgument(RXObject_t* expression) {
	   RXObject_t* message = eina_array_data_get(RXParser_stack, RXParser_stackTop);
	   RXList_append(message, expression);
	}

	static RXObject_t* RXParser_messageWithName(RXObject_t* name) {
		RXObject_t* result = RXList_spawn(RXMessage_o, NULL);
		RXObject_setSlot(result, RXSymbol_name_o, name);
		return result;
	}

	static void RXParser_pushOperator(RXObject_t* name) {
		RXParser_push(RXParser_messageWithName(name));
		RXParser_push(RXList_spawn(RXExpression_o, NULL));
	}

	static void RXParser_completeBinaryExpression(void) {
		 RXParser_appendArgument(RXParser_pop());
		 RXParser_appendMessage(RXParser_pop());
	}

	static RXObject_t* RXParser_parse(pANTLR3_INPUT_STREAM input) {
	   	RXParser_setup();
	   	RXParser_push(RXList_spawn(RXExpression_o, NULL));
	   
		assert(input != NULL);
		pRXLexer lexer = RXLexerNew(input);
		assert(lexer != NULL);
		pANTLR3_COMMON_TOKEN_STREAM tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
		assert(tstream != NULL);
	   	pRXParser parser = RXParserNew(tstream);
	   	parser->file(parser);
	   
	   	RXObject_t* result = RXParser_pop();
	   	RXParser_clean();
	   	return result;
	}

	RXObject_t* RXParser_expressionFromCString(char* string) {
	   return RXParser_parse(antlr3NewAsciiStringInPlaceStream(string, strlen(string), NULL));
	}

	RXObject_t* RXParser_expressionFromCFile(char* fileName) {
	   	return RXParser_parse(antlr3AsciiFileStreamNew(fileName));
	}
}

file:
	expression EOF
	;

expression:
    NL* expression_body NL*
    ;

expression_body:
	or_expression (
		(';' | NL)+ { RXParser_appendMessage(RXParser_messageWithName(RXSymbol_semicolon_o)); }
		or_expression
	)*
   	;

or_expression:
    and_expression (
    	'or' { RXParser_pushOperator(RXSymbol_or_o); }
    	NL* and_expression { RXParser_completeBinaryExpression(); }
    )*
    ;

and_expression:
    comparison_expression (
    	'and' { RXParser_pushOperator(RXSymbol_and_o); }
    	NL* comparison_expression { RXParser_completeBinaryExpression(); }
    )*
    ;

comparison_expression:
	additive_expression (
		comparison_operator
		NL* additive_expression { RXParser_completeBinaryExpression(); }
	)?
    ;

comparison_operator:
      '==' { RXParser_pushOperator(RXSymbol_equal_o); } 
    | '!=' { RXParser_pushOperator(RXSymbol_notEqual_o); }
    | '<'  { RXParser_pushOperator(RXSymbol_lessThan_o); }
    | '>'  { RXParser_pushOperator(RXSymbol_greaterThan_o); }
    | '<=' { RXParser_pushOperator(RXSymbol_lessOrEqual_o); }
    | '>=' { RXParser_pushOperator(RXSymbol_greaterOrEqual_o); }
    ;

additive_expression:
	multiplicative_expression (
		additive_operator
		NL* multiplicative_expression { RXParser_completeBinaryExpression(); }
	) 
    ;

additive_operator:
      '+' { RXParser_pushOperator(RXSymbol_add_o); }
    | '-' { RXParser_pushOperator(RXSymbol_subtract_o); }
    ;

multiplicative_expression:
	unary_expression (
		multiplicative_operator
		NL* unary_expression { RXParser_completeBinaryExpression(); }
	)*
    ;

multiplicative_operator:
      '*' { RXParser_pushOperator(RXSymbol_multiply_o); }
    | '/' { RXParser_pushOperator(RXSymbol_divide_o); }
    ;

unary_expression:
    primary_expression
    | '+' primary_expression  { RXParser_appendMessage(RXParser_messageWithName(RXSymbol_unaryPlus_o)); }
    | '-' primary_expression { RXParser_appendMessage(RXParser_messageWithName(RXSymbol_negate_o)); }
    ;

primary_expression:
    first_receiver message_or_assignment*
    ;

first_receiver:
      message_or_assignment
    | STRING { RXParser_appendMessage(RXSymbol_symbolForCString((char*)$STRING.text)); }
    | DECIMAL { RXParser_appendMessage(RXInteger_spawn(RXInteger_o, strtol((char*)$DECIMAL.text, NULL, 10))); }
    | HEX { RXParser_appendMessage(RXInteger_spawn(RXInteger_o, strtol((char*)$HEX.text, NULL, 16))); }
//    | r=REAL { RXParser_appendMessage(RXReal_spawn(RXReal_o, r)); } // TODO
    | '(' expression ')'
   ;

message_or_assignment
	@init { RXObject_t* symbol; }:
	IDENTIFIER { symbol = RXSymbol_symbolForCString((char*)$IDENTIFIER.text); }
	(
      	  /* Empty */ { RXParser_appendMessage(RXParser_messageWithName(symbol)); }
    	| '(' { RXParser_push(RXParser_messageWithName(symbol)); } argument_list ')' {
         		RXParser_appendMessage(RXParser_pop());
      		}
    	| '=' {
		     	RXParser_push(RXParser_messageWithName(RXSymbol_setSlot_o));
		     	RXParser_push(RXList_spawn(RXExpression_o, NULL));
		     	RXParser_appendMessage(symbol);
		     	RXParser_appendArgument(RXParser_pop());
		     	RXParser_push(RXList_spawn(RXExpression_o, NULL));
      		} or_expression {
		     	RXParser_appendArgument(RXParser_pop());
		     	RXParser_appendMessage(RXParser_pop());
      		}
	)
   ;

argument_list:
      NL* { RXParser_pop(); }
    | argument (',' argument)*
   ;

argument:
	{ RXParser_push(RXList_spawn(RXExpression_o, NULL)); }
   	expression { RXParser_appendArgument(RXParser_pop()); }
   	;

// Lexer ---------------------------------------------------------------

// TODO add whitespace and comments

NL:
	'\r'? '\n'
	;

fragment LETTER:
	'a'..'Z' | 'A'..'Z'
	;
	
fragment ULETTER:
	LETTER | '_'
	;
	
fragment DIGIT:
	'0'..'9'
	;
	
fragment HEXDIGIT:
	  '0'..'9'
	| 'a'..'f'
	| 'A'..'F'
	;
	
IDENTIFIER:
	(ULETTER (ULETTER | DIGIT)*)
	;

DECIMAL:
	d=DIGIT+
	;

HEX:
	'0x' HEXDIGIT+
	;
	
fragment ESCAPE:
	'\\' (UESCAPE |'b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
	;
	
fragment UESCAPE
	: 'u' HEXDIGIT HEXDIGIT HEXDIGIT HEXDIGIT
	;
	
STRING:
	'"' s=(( ESCAPE | ~('\u0000'..'\u001f' | '\\' | '\"' ) )*) '"'
	;
	
