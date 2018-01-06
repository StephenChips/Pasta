type token =
  | ICONST of (int)
  | RCONST of (float)
  | BCONST of (bool)
  | CCONST of (char)
  | SCONST of (string)
  | ID of (string)
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
  | GETREF
  | DEREF
  | COLON
  | SCOLON
  | COMMA
  | DOT
  | RANGE
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
  | AS
  | RAISE
  | EXCEPTION
  | FINALLY
  | FALSE
  | TRUE
  | OR
  | AND

val program :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> unit
