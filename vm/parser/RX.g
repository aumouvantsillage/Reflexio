
grammar RX;

options {
	language = C;
}

@header {
	#include <core/RXCore.h>
	#include <stdio.h>
	
	RXObject_t* RXParser_expressionFromCString(char* string);
	RXObject_t* RXParser_expressionFromCFile(char* fileName);
}

@members {
	#include <lib/RXLib.h>
	#include <antlr3.h>
	#include "RXLexer.h"
	
	static RXObject_t* RXParser_parse(pANTLR3_INPUT_STREAM input) {
		assert(input != NULL);
		pRXLexer lexer = RXLexerNew(input);
		assert(lexer != NULL);
		pANTLR3_COMMON_TOKEN_STREAM tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lexer));
		assert(tstream != NULL);
	   	pRXParser parser = RXParserNew(tstream);
	   	RXObject_t* result = parser->file(parser);
	   	// printf("\%s\n", RXObject_respondTo(result, RXSymbol_asSource_o, RXNil_o, 0));
	   	return result;
	}

	static RXObject_t* RXParser_appendMessageWithName(RXObject_t* expr, RXObject_t* name) {
		RXObject_t* msg = RXList_spawn(RXMessage_o, NULL);
		RXObject_setSlot(msg, RXSymbol_name_o, name);
		RXList_append(expr, msg);
		return msg;
	}
	
	static RXObject_t* RXParser_appendOperation(RXObject_t* expr, RXObject_t* name) {
		RXObject_t* msg = RXParser_appendMessageWithName(expr, name);
		RXObject_t* arg = RXList_spawn(RXExpression_o, NULL);
		RXList_append(msg, arg);
		return arg;
	}
	
	RXObject_t* RXParser_expressionFromCString(char* string) {
	   return RXParser_parse(antlr3StringStreamNew(string, ANTLR3_ENC_8BIT, strlen(string), NULL));
	}

	RXObject_t* RXParser_expressionFromCFile(char* fileName) {
	   	return RXParser_parse(antlr3FileStreamNew(fileName, ANTLR3_ENC_8BIT));
	}
}

file returns[RXObject_t* expr]:
	{ expr = RXList_spawn(RXExpression_o, NULL); }
	expression[expr] EOF
	;

expression[RXObject_t* expr]:
    NL* expression_body[expr] NL*
    ;

expression_body[RXObject_t* expr]:
	statement[expr] ( 
		(';' | NL)+ {
			RXParser_appendMessageWithName(expr, RXSymbol_semicolon_o);
		}
		statement[expr]
	)*
   	;

statement[RXObject_t* expr]:
	  (assignment_prefix[NULL] IDENTIFIER '=') => assignment[expr]
	| or_expression[expr]
	;

assignment[RXObject_t* expr]
	@init { RXObject_t* arg; } :
	assignment_prefix[expr] IDENTIFIER '=' NL* {
		RXObject_t* msg = RXParser_appendMessageWithName(expr, RXSymbol_setSlot_o);
		RXList_append(msg, RXSymbol_symbolForCString($IDENTIFIER.text->chars));
		arg = RXList_spawn(RXExpression_o, NULL);
		RXList_append(msg, arg);
	} or_expression[arg]
  	;

assignment_prefix[RXObject_t* expr]:
	first_receiver[expr]? message[expr]*
	;

or_expression[RXObject_t* expr]
	@init { RXObject_t* arg; } :
    and_expression[expr] (
    	'or' NL* {
    		arg = RXParser_appendOperation(expr, RXSymbol_or_o);
    	}
    	and_expression[arg]
    )*
    ;

and_expression[RXObject_t* expr]
	@init { RXObject_t* arg; } :
    comparison_expression[expr] (
    	'and' NL* {
    		arg = RXParser_appendOperation(expr, RXSymbol_and_o);
    	}
    	comparison_expression[arg]
    )*
    ;

comparison_expression[RXObject_t* expr]
	@init { RXObject_t* arg; } :
	additive_expression[expr] (
		op=comparison_operator NL* {
			arg = RXParser_appendOperation(expr, $op.symbol);
		}
		additive_expression[arg]
	)?
    ;

comparison_operator returns[RXObject_t* symbol]:
      '==' { symbol = RXSymbol_equal_o; } 
    | '!=' { symbol = RXSymbol_notEqual_o; }
    | '<'  { symbol = RXSymbol_lessThan_o; }
    | '>'  { symbol = RXSymbol_greaterThan_o; }
    | '<=' { symbol = RXSymbol_lessOrEqual_o; }
    | '>=' { symbol = RXSymbol_greaterOrEqual_o; }
    ;

additive_expression[RXObject_t* expr]
	@init { RXObject_t* arg; } :
	multiplicative_expression[expr] (
		op=additive_operator NL* {
			arg = RXParser_appendOperation(expr, $op.symbol);
		}
		multiplicative_expression[arg]
	)*
    ;

additive_operator returns[RXObject_t* symbol]:
      '+' { symbol = RXSymbol_add_o; }
    | '-' { symbol = RXSymbol_subtract_o; }
    ;

multiplicative_expression[RXObject_t* expr]
	@init { RXObject_t* arg; } :
	unary_expression[expr] (
		op=multiplicative_operator NL* {
			arg = RXParser_appendOperation(expr, $op.symbol);
		}
		unary_expression[arg]
	)*
    ;

multiplicative_operator returns[RXObject_t* symbol]:
      '*' { symbol = RXSymbol_multiply_o; }
    | '/' { symbol = RXSymbol_divide_o; }
    ;

unary_expression[RXObject_t* expr]:
    primary_expression[expr]
    | '+' primary_expression[expr] { RXParser_appendMessageWithName(expr, RXSymbol_unaryPlus_o); }
    | '-' primary_expression[expr] { RXParser_appendMessageWithName(expr, RXSymbol_negate_o); }
    ;

primary_expression[RXObject_t* expr]:
      first_receiver[expr] message[expr]*
    | message[expr]+
    ;

first_receiver[RXObject_t* expr]:
      STRING  { RXList_append(expr, RXSymbol_symbolForCString($STRING.text->subString($STRING.text, 1, $STRING.text->len-1)->chars)); }
    | DECIMAL { RXList_append(expr, RXInteger_spawn(RXInteger_o, strtol($DECIMAL.text->chars, NULL, 10))); }
    | HEX     { RXList_append(expr, RXInteger_spawn(RXInteger_o, strtol($HEX.text->subString($HEX.text, 2, $HEX.text->len)->chars, NULL, 16))); }
    | '(' expression[expr] ')'
   ;

message[RXObject_t* expr]
	@init { RXObject_t* msg; } :
	IDENTIFIER {
  		msg = RXParser_appendMessageWithName(expr, RXSymbol_symbolForCString($IDENTIFIER.text->chars));
  	}
  	( '(' argument_list[msg] ')' )?
	;

argument_list[RXObject_t* msg]:
      NL*
    | argument[msg] (',' argument[msg])*
   ;

argument[RXObject_t* msg]
	@init { RXObject_t* arg; } :
	{
		arg = RXList_spawn(RXExpression_o, NULL);
		RXList_append(msg, arg);
	}
   	expression[arg]
   	;

// Lexer ---------------------------------------------------------------

fragment LETTER:
	'a'..'z' | 'A'..'Z'
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
	(ULETTER | '$') (ULETTER | DIGIT)*
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

LCOMMENT:
	'//' ~'\n'*	{ $channel = HIDDEN; }
	;

MLCOMMENT:
	'/*' (options{greedy=false;} : .)* '*/' { $channel = HIDDEN; }
	;

NL:
	'\r'? '\n'
	;

WS:
	(' ' | '\t') { $channel = HIDDEN; }
	;
