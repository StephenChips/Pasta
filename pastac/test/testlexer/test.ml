open Parser;;

let token_to_string token = 
  match token with
  | ICONST s -> String.concat "" ["const["; (string_of_int s); "]"]
  | RCONST f -> String.concat "" ["const["; (string_of_float f); "]"]
  | BCONST b -> String.concat "" ["const["; (string_of_bool b); "]"]
  | CCONST c -> String.concat "" ["const["; (String.make 1 c); "]"]
  | SCONST s -> String.concat "" ["const["; s ;"]"]
  | ID s -> String.concat "" ["id["; s; "]"]
  | ADD -> "ADD"
  | SUB -> "SUB"
  | MUL -> "MUL"
  | DIV -> "DIV"
  | MOD -> "MOD"
  | ASG -> "ASG"
  | LPAR -> "LPAR"
  | RPAR -> "RPAR"
  | LBRA -> "LBRA"
  | RBRA -> "RBRA"
  | GT -> "GT"
  | LT -> "LT"
  | EQUAL -> "EQUAL"
  | NOTEQ -> "NOTEQ"
  | GTEQ -> "GTEQ"
  | LTEQ -> "LTEQ"
  | GETREF -> "GETREF"
  | DEREF -> "DEREF"
  | COLON -> "COLON"
  | SCOLON -> "SCOLON"
  | COMMA -> "COMMA"
  | DOT -> "DOT"
  | RANGE -> "RANGE"
  | PROGRAM -> "PROGRAM"
  | FUNCTION -> "FUNCTION"
  | PROCEDURE -> "PROCEDURE"
  | BEGIN -> "BEGIN"
  | END -> "END"
  | FOR -> "FOR" 
  | WHILE -> "WHILE"
  | REPEAT -> "REPEAT"
  | INTEGER -> "INTEGER"
  | THEN -> "THEN"
  | UNTIL -> "UNTIL"
  | IF -> "IF"
  | ELSE -> "ELSE"
  | INC -> "INC"
  | DEC -> "DEC"
  | DO -> "DO"
  | REF -> "REF"
  | REAL -> "REAL"
  | STRING -> "STRING"
  | BOOLEAN -> "BOOLEAN"
  | CHARACTER -> "CHARACTER"
  | ARRAY -> "ARRAY"
  | TO -> "TO"
  | DOWNTO -> "DOWNTO"
  | VAR -> "VAR"
  | TYPE -> "TYPE"
  | ENUMERATE -> "ENUMERATE"
  | RECORD -> "RECORD"
  | WRITE -> "WRITE"
  | READ -> "READ"
  | CASE -> "CASE"
  | OF -> "OF"
  | BREAK -> "BREAK"
  | CONTINUE -> "CONTINUE"
  | TRY -> "TRY"
  | EXCEPT -> "EXCEPT"
  | NIL -> "NIL"
  | AS -> "AS"
  | RAISE -> "RAISE"
  | EXCEPTION -> "EXCEPTION"
  | FINALLY -> "FINALLY"
  | FALSE -> "FALSE"
  | TRUE -> "TRUE"
  | OR -> "OR"
  | AND -> "AND"


let rec assert_tokens_equal expect actual =
  match expect, actual with
  | [], []                 -> true
  | a::rexpect, b::ractual ->
      if a = b then assert_tokens_equal rexpect ractual else false
  | a::rexpect, []         -> false
  | [], b::ractual         -> false
;; 

let lexbuf = Lexing.from_channel stdin in
while true do
   print_endline (token_to_string (Lexer.token lexbuf))
done
;;
