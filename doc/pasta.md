(* lexer rules *)
type Token = 
    | ICONST of int 
    | RCONST of float
    | BCONST of boolean
    | SCONST of string
    | CCONST of char
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
    | GETADDR
    | DEREF
    | COLON
    | SCOLON
    | COMMA
    | DOT
    | RANGE
    | AS
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
    | NIL


(* parser rules *)

(** STATEMENTS **)
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
    | {write-statement}
    | {read-statement}
    | {inc-statement}
    | {dec-statement}
    | BREAK
    | CONTINUE
    | (* empty statement *)

(*
    change array syntax
    array [] of type 

    type should not be an array.
*)
{assignment-statement} ::= ID ASG {expression}

{procedure-statement} ::= ID LPAR {expression-list} RPAR

{compound-statement} ::= BEGIN {statement-list} END

{case-of-statement} ::=
    | CASE {expression} OF {case-part} {else-part} END
    | CASE {expression} OF {case-part} END

(* sematic check: literals in list should have same type *)
{case-part} ::= 
    | {literal-or-enum-list} COLON {statement} SCOLON {case-part}
    | {literal-or-enum-list} COLON {statement} SCOLON 

{literal-or-enum} ::=
    | {literal}
    | ID

{literal} ::=
    | ICONST
    | RCONST
    | BCONST
    | SCONST
    | CCONST

{literal-or-enum-list} ::=
    | {literal-or-enum} {literal-or-enum-list}
    | {literal-or-enum}

{else-part} ::= ELSE {statement-list}

{if-then-else-statement} ::=
    | IF {expression} THEN {statement}
    | IF {expression} THEN {statement} ELSE {other-condition}

{other-condition} ::=
    | {statement}
    | IF {expression} THEN {statement}
    | IF {expression} THEN {statement} ELSE {other-condition}

{while-do-statement} ::= 
    | WHILE {expression} DO {statement}

{for-statement} ::=
    | FOR ID ASG {expression} TO {expression} DO {statement}
    | FOR ID ASG {expression} DOWNTO {expression} DO {statement}

{repeat-until-statement} ::= 
    | REPEAT {statement} UNTIL {expression}

{write-statement} ::= WRITE {expression-list}

{read-statement} ::= READ {expression-list}

{inc-statement} ::= INC {expression}

{dec-statement} ::= DEC {expression}

(* {expression-list} is defined in expression part *)
{raise-statement} ::= RAISE ID LPAR {expression-list} RPAR 

{try-except-satement} ::=
    | TRY {statement-list} EXCEPT {exception-part} DO {statement-list} END
    | TRY {statement-list} FINALLY {statement-list} END
    | TRY {statement-list} EXCEPT {exception-part} DO {statement-list} FINALLY {statement-list} END

{exception-part} ::=
    | {id-list}
    | {id-list} AS ID

{id-list} ::=
    | ID COLON {id-list}
    | ID

{statement-list} ::=
    | {statement}
    | {statement} SCOLON {statement-list}
    | (* empty list *)

(** EXPRESSION **)
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
    | GETADDR {not-expression}    (* get object's address *)
    | DEREF {not-expression}  (* access refered object's value *)
    | {primitive-expression}

{primitive-expression} ::= 
    | TRUE
    | FALSE
    | REAL
    | INTEGER
    | ID
    | NIL
    | LPAR {expression} RPAR
    | ID LBRA {expression-list} RBRA  (* array indexing, should has one expression in the list at less *)
    | ID LPAR {expression-list} RPAR  (* function call *)
    | ID DOT {field}   (* access record or exception instance's field *)

{field} ::= 
    | ID
    | ID DOT {field}

{expression-list} ::= 
    | {expression}
    | {expression} COMMA {expression-list}
    | (* empty list *)

(** DECLARATIONS AND DEFINITIONS **)
{simple-type} ::=
    | INTEGER
    | REAL
    | STRING
    | CHARACTER
    | BOOLEAN

{any-named-type} ::=
    | ID
    | {simple-type}

{ordinal-type} ::=
    | INTEGER
    | REAL
    | CHARACTER
    | BOOLEAN

{any-type} ::=
    | ID (* any type except simple type *)
    | {array-type}
    | {simple-type}

{array-type} ::= ARRAY LBRA {range-list} RBRA OF {any-named-type}

{range-list} ::=
    | {range} COMMA {range-list}
    | {range}

{range} ::=
    | INT RANGE INT
    | (* empty range *)

{variable-declaration} ::= VAR {var-declrs-and-inits}

{var-declrs-and-inits} ::=
    | {var-declr-list} {var-declrs-or-inits}
    | {var-init-list} {var-declrs-or-inits}
    | {var-declr-list}
    | {var-init-list}

{var-declr-list} ::= 
    | {var-declr} SCOLON {var-declr-list}
    | {var-declr}

{var-declr} ::= 
    | {id-list} COLON {any-type}

{var-init-list} ::= 
    | {var-init} SCOLON {var-init-list}
    | {var-init}

(* the result of expression should be a constant *)
(* the variable should has simple type *)

{var-init} ::= 
    | ID COLON {simple-type} EQ {expression} 
    | ID COLON ID EQ ID (* enumerate type *)

{procedure-definition} ::= PROCEDURE ID LPAR {parameter-list} RPAR SCOLON {local-var-declrs} {compound-statement} SCOLON

{function-definition} ::= FUNCTION ID LPAR {parameter-list} RPAR COLON {any-type} SCOLON {local-var-declrs} {compund-statement} SCOLON

{local-var-declrs} ::=
    | {var-declrs-or-inits}
    | (* no variable declared *)

{parameter-list} ::=
    | {var-declr-list} SCOLON {var-init-list}
    | {var-declr-list}
    | {var-init-list}
    | (* empty list *)

{type-definition} ::= TYPE {type-definition-list}

{type-definition-list} ::= 
    | ID EQ {type-body} SCOLON {type-definition-list}
    | ID EQ {type-body} SCOLON

{type-body} ::=
    | {any-type}   (* alias a type *)
    | {record-body}
    | {enumerate-body}
    | {exception-body}
    | {reference-body}

{record-body} ::=
    | RECORD {fixed-field-list} {variant-part} END
    | RECORD {variant-part} END
    | RECORD {fixed-field-list} END

{fixed-field-list} ::=
    | {fixed-field} SCOLON {fixed-field-list}
    | {fixed-field}

{fixed-field} ::= {id-list} COLON {any-type}

{variant-part} ::=
    | CASE {literal-or-enum-list} COLON {ordinal-type} OF {fixed-field-list} {variant-else-part} END
    | CASE {literal-or-enum-list} COLON {ordinal-type} OF {fixed-field-list} END

{variant-else-part} ::= ELSE {fixed-field-list} 

{enumerate-type} ::= ENUMERATE {id-list} END

{exception-type} ::=
    | EXCEPTION
    | EXCEPTION {fixed-field-list} END

{reference-type} ::= REF {any-type}

(* program structure *)
{program} ::= {program-declaration} {global-declaration-list} {compound-statement} DOT

{global-declaration-list} ::=
    | {global-declaration} SCOLON {global-declaration-list}
    | {global-declaration} SCOLON

{global-declaration} ::=
    | {variable-declaration}
    | {procedures-definitions}
    | {functions-definitions}
    | {type-definitions}

{program-declaration} ::= 
    | PROGRAM identifier SCOLON

