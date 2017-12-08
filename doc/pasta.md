(* lexer rules *)
type Token = 
    | INT of int
    | REAL of float
    | BOOL of boolean
    | STRING of string
    | KEYWORD of string
    | ADD
    | SUB
    | MUL
    | DIV
    | MOD
    | ASG
    | LPAR
    | RPAR
    | LBRA
    | RBRA
    | GT
    | LT
    | EQUAL
    | NOTEQ
    | GTEQ
    | LTEQ
    | REF
    | DEREF
    | COLON
    | SCOLON
    | COMMA
    | DOT
    | RANGE
(* keywords *)
    | PROGRAM
    | FUNCTION
    | PROCEDURE
    | BEGIN
    | END
    | FOR
    | WHILE
    | REPEAT
    | INTEGER
    | THEN
    | UNTIL
    | IF
    | ELSE
    | INC
    | DEC
    | DO
    | REF
    | REAL
    | STRING
    | BOOLEAN
    | CHARACTER
    | ARRAY
    | TO
    | DOWNTO
    | VAR
    | TYPE
    | ENUMERATE
    | RECORD
    | WRITE
    | READ
    | CASE
    | OF 
    | BREAK
    | CONTINUE
    | TRY
    | EXCEPT


(* parser rules *)

(* statements *)
{statement} ::=
    | {assignment-statement}
    | {procedure-statement}
    | {if-then-else-statement}
    | {case-of-statement}
    | {while-do-statement}
    | {for-statement}
    | {repeat-until-statement}
    | {try-except-statement}
    | {compound-statement}
    | {raise-statement}
    | {empty-statement}
    | BREAK
    | CONTINUE

{assignment-statement} ::= 
    | ID ASG {expression}

{procedure-statement} ::= ID LPAR {expression-list} RPAR

{compound-statement} ::=
    | BEGIN {statement-list} END

{statement-list} ::=
    | {statement} ; {statement-list}
    | {statement}

{case-of-statement} ::=
    | CASE {expression} OF {case-part} {else-part} END
    | CASE {expression} OF {case-part} END

{case-part} ::= 
    | {case} SCOLON {statement} {case-part}
    | {case} SCOLON {statement}

{case} ::=
    | {literals} COLON {case}
    | {literals}

{literals} ::=
    | INT
    | REAL
    | STRING
    | BOOLEAN
    | ID    (* enumerate type *)

{else-part} ::= ELSE {statement-list}

{if-then-else-statement} ::=
    | IF {expression} THEN {statements}
    | IF {expression} THEN {statements} ELSE {other-condition}

{other-condition} ::=
    | {statement}
    | IF {expression} THEN {statements}
    | IF {expression} THEN {statements} ELSE {rest-condition}

{while-do-statement} ::= 
    | WHILE {expression} DO {statement}

{for-statement} ::=
    | FOR ID ASG {expression} TO {expression} DO {statement}
    | FOR ID ASG {expression} DOWNTO {expression} DO {statement}

{repeat-until-statement} ::= 
    | REPEAT {statements} UNTIL {expression}

(* {expression-list} is defined in expression part *)
{raise-statement} ::= RAISE ID LPAR {expression-list} RPAR 

{try-except-satement} ::=
    | TRY {statement-list} EXCEPT {exception-part} DO {statement-list} END
    | TRY {statement-list} FINALLY {statement-list} END
    | TRY {statement-list} EXCEPT {exception-part} DO {statement-list} FINALLY {statement-list} end;

{exception-part} ::=
    | {id-list}
    | {id-list} AS ID

{id-list} ::=
    | ID {id-list}
    | ID

{compound-statement} ::=
    | BEGIN {statement-list} END

{statement-list} ::=
    | {statement}
    | {statement} SCOLON {statement-list}

{empty-statement} ::= 


(* expression *)
{expression} ::= 
    | {additional-expression} GT   {expression}
    | {additional-expression} LT   {expression}
    | {additional-expression} EQ   {expression}
    | {additional-expression} GTEQ {expression}
    | {additional-expression} LTEQ {expression}
    | {additional-expression} NOTEQ {expression}
    | {additional-expression}

{additional-expression} ::= 
    | {multiplicative-expression} ADD {additional-expression} |
    | {multiplicative-expression} SUB {additional-expression} |
    | {multiplicative-expression} OR  {additional-expression} |
    | {multiplicative-expression}

{multiplicative-expression} ::= 
    | {not-expression} MUL {multiplicative-expression}
    | {not-expression} DIV {multiplicative-expression}
    | {not-expression} MOD {multiplicative-expression}
    | {not-expression} AND {multiplicative-expression}
    | {not-expression}

{not-expression} ::=
    | NOT {primitive-expression}
    | REF {not-expression}    (* get object address *)
    | DEREF {not-expression}  (* access refered object *)
    | {primitive-expression}

{primitive-expression} ::= 
    | TRUE
    | FALSE
    | REAL
    | INTEGER
    | ID
    | LPAR {expression} RPAR
    | ID LBRA {expression-list} RBRA  (* array indexing *)
    | ID LPAR {expression-list} RPAR  (* function call *)
    | ID DOT {field}   (* access record or exception instant field *)

{field} ::= 
    | ID
    | ID DOT {field}

{expression-list} ::= 
    | {expression}
    | {expression} COMMA {expression-list}

(* declarations and definitions *)
{simple-type} ::=
    | INTEGER
    | REAL
    | STRING
    | BOOLEAN

{any-type} ::=
    | ID
    | {simple-type}

{variable-declaration} ::= VAR {variable-declaration-list}

{variable-declaration-list} ::=
    | {variable-declaration} SCOLON {variable-declaration-list}
    | {variable-initializer} SCOLON {variable-declaration-list}
    | {variable-declaration} SCOLON
    | {variable-initializer} SCOLON

{variable-declaration} ::=
    | {identifier-list} COLON {any-type}

{identifier-list} ::= 
    | identifier , {identifier-list}
    | identifier

// should only a constant expression, no variables and function calls will involved!
{variable-initializer} ::= 
    | ID COLON {any-type} = {expression} 


{procedure-definition} ::=
    | PROCEDURE ID LPAR {parameter-sequence} RPAR SCOLON {variable-declaration} {compound-statement} SCOLON

{function-definition} ::= 
    | FUNCTION ID LPAR {parameter-sequence} RPAR COLON {any-type} COLON {vaiable-declaration} {compund-statement} SCOLON

{type-definition} ::= TYPE {type-definition-list}

{type-definition-list} ::= ID EQ {type-body}

{type-body} ::=
    | {any-type}   (* alias a type *)
    | {record-type}
    | {array-type}
    | {enumerate-type}
    | {exception-type}

{any-type} ::=
    | ID
    | {simple-type}

{simple-type} ::=
    | INTEGER
    | REAL
    | STRING
    | BOOLEAN
    | ID (* enumberate variable *)

{record-type} ::=
    | RECORD {fixed-field-list} {variant-part} END
    | RECORD {fixed-field-list} END

{fixed-field-list} ::=
    | {id-list} COLON {any-type} SCOLON {fixed-field-list}
    | {id-list} COLON {any-type} SCOLON

{variant-part} ::=
    | CASE ID COLON {simple-type} OF {fixed-field-list} {variant-else-part} END
    | CASE ID COLON {simple-type} of {fixed-field-list} END

{variant-else-part} ::= ELSE {fixed-field-list} 


{array-type} ::= 
    | {static-array-type}
    | {dynamic-array-type}

{static-array-type} ::= ARRAY LBRA {range-list} RBRA OF {any-type}

{range-list} ::=
    | INT RANGE INT {range-list}
    | INT RANGE INT

{dynamic-array-type} ::=
    | ARRAY OF {any-type}

(* program structure *)
{program} ::= 
    | {program-declaration} {global-variable-declaration} {procedure-and-function-definition-sequence} {main-block} DOT

{program-declaration} ::= 
    | program identifier SCOLOn

{global-variable-declaration} ::= 
    | {variable-declaration-block}

{procedure-and-function-definition-sequence} ::=
    | {procedure-and-function-definition} {procedure-and-function-definition-sequence}
    | {procedure-and-function-definition}

{procedre-and-function-definition} ::=
    | {procedure-definition}
    | {function-definition}

{parameter-sequence} ::=
    | {variable-declaration} scolon {rest-parameter-sequence}
    | {variable-declaration}
    | epsilon

{rest-parameter-sequence} ::=
    | {variable-declaration} scolon {variable-declaration}
    | {variable-declaration}
